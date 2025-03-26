#include "Tasks/TimeLimitedQuestTask.h"

#include "Data/Tasks/QuestTaskTimeLimitedInfoDataAsset.h"
#include "QuestCoreTypes.h"
#include "QuestSubsystem.h"

float UTimeLimitedQuestTask::GetRemainingTime() const
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::AddRemainingTime: World is not valid!"));
		return 0.f;
	}

	const bool bIsTimerActive = World->GetTimerManager().IsTimerActive(TaskLimitTimerHandle);

	return bIsTimerActive ? World->GetTimerManager().GetTimerRemaining(TaskLimitTimerHandle) : 0.f;
}

void UTimeLimitedQuestTask::AddRemainingTime(float TimeToAdd)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::AddRemainingTime: World is not valid!"));
		return;
	}
	
	if (FMath::IsNearlyZero(TimeToAdd) || !World->GetTimerManager().IsTimerActive(TaskLimitTimerHandle))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::AddRemainingTime: TaskLimitTimerHandle is not valid or TimeToAdd is equal to 0!"));
		return;
	}

	const float RemainingTime = GetRemainingTime();
	World->GetTimerManager().SetTimer(TaskLimitTimerHandle, this, &UTimeLimitedQuestTask::OnTaskTimeLimitReached, RemainingTime + TimeToAdd, false);

	OnTaskRemainingTimeUpdated.Broadcast(RemainingTime + TimeToAdd);
}

void UTimeLimitedQuestTask::OnTaskStarted()
{
	if (TaskLimitTime <= 0)
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::OnTaskStarted: TaskLimitTime is less or equal to 0!"));
		return;
	}
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::OnTaskStarted: World is not valid!"));
		return;
	}

	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UTimeLimitedQuestTask::OnTaskStarted: QuestSubsystem is not valid!"));
		return;
	}

	StartTime = (float)FPlatformTime::Seconds();
	SavedPassedTime = FMath::IsNearlyZero(SavedRemainingTime) ? 0.f : SavedPassedTime;

	const float CurrentLimitTime = bShouldFinishOnInterrupt || FMath::IsNearlyZero(SavedRemainingTime) ? TaskLimitTime : SavedRemainingTime;
	World->GetTimerManager().SetTimer(TaskLimitTimerHandle, this, &UTimeLimitedQuestTask::OnTaskTimeLimitReached, CurrentLimitTime, false);

	QuestSubsystem->GetOnQuestsWorldCleanupDelegate().AddUObject(this, &ThisClass::OnWorldCleanup);
}

void UTimeLimitedQuestTask::OnTaskCompleted(EQuestCompleteRelust CompleteResult)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::OnTaskFinished: World is not valid!"));
		return;
	}
	
	World->GetTimerManager().ClearTimer(TaskLimitTimerHandle);
	
	SavedRemainingTime = 0.f;
	
	float EndTime = (float)FPlatformTime::Seconds();
	SavedPassedTime += EndTime - StartTime;
}

void UTimeLimitedQuestTask::OnWorldCleanup(UWorld* World)
{
	if (!IsValid(World))
	{
		return;
	}
		
	if (bShouldFinishOnInterrupt)
	{
		FinishTask(EQuestCompleteRelust::Cancelled);
	}
	else
	{
		PreSaveGame();
		World->GetTimerManager().ClearTimer(TaskLimitTimerHandle);
	}
}

void UTimeLimitedQuestTask::FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset)
{
	const UQuestTaskTimeLimitedInfoDataAsset* TimeLimitedInfoDataAsset = Cast<UQuestTaskTimeLimitedInfoDataAsset>(QuestInfoDataAsset);
	if (!IsValid(TimeLimitedInfoDataAsset))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::FillTaskInfo: QuestInfoDataAsset is not of type UQuestTaskTimeLimitedInfoDataAsset!"));
		return;
	}

	TaskLimitTime = TimeLimitedInfoDataAsset->TimeLimit;
	TimeOverCompleteResult = TimeLimitedInfoDataAsset->TimeOverCompleteResult;
	bShouldFinishOnInterrupt = TimeLimitedInfoDataAsset->bShouldFinishOnInterrupt;
}

void UTimeLimitedQuestTask::PreSaveGame()
{
	SavedRemainingTime = GetRemainingTime();
	SavedPassedTime += (FPlatformTime::Seconds() - StartTime);
}

void UTimeLimitedQuestTask::OnTaskTimeLimitReached()
{
	FinishTask(TimeOverCompleteResult);
}

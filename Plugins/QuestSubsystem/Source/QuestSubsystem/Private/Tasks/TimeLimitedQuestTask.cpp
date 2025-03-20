#include "Tasks/TimeLimitedQuestTask.h"

#include "Data/Tasks/QuestTaskTimeLimitedInfoDataAsset.h"
#include "QuestCoreTypes.h"

float UTimeLimitedQuestTask::GetRemainingTime() const
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::AddRemainingTime: World is not valid!"));
		return 0;
	}

	return World->GetTimerManager().GetTimerRemaining(TaskLimitTimerHandle);
}

void UTimeLimitedQuestTask::AddRemainingTime(float TimeToAdd)
{
	if (!TaskLimitTimerHandle.IsValid() || TimeToAdd == 0)
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::AddRemainingTime: TaskLimitTimerHandle is not valid or TimeToAdd is equal to 0!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::AddRemainingTime: World is not valid!"));
		return;
	}

	const float RemainingTime = World->GetTimerManager().GetTimerRemaining(TaskLimitTimerHandle);
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

	World->GetTimerManager().SetTimer(TaskLimitTimerHandle, this, &UTimeLimitedQuestTask::OnTaskTimeLimitReached, TaskLimitTime, false);
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &ThisClass::OnWorldCleanup);
}

void UTimeLimitedQuestTask::OnTaskCompleted(EQuestTaskCompleteResult CompleteResult)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UTimeLimitedQuestTask::OnTaskFinished: World is not valid!"));
		return;
	}

	World->GetTimerManager().ClearTimer(TaskLimitTimerHandle);
}

void UTimeLimitedQuestTask::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(TaskLimitTimerHandle);

		OnTaskTimeLimitReached();
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
}

void UTimeLimitedQuestTask::OnTaskTimeLimitReached()
{
	FinishTask(TimeOverCompleteResult);
}
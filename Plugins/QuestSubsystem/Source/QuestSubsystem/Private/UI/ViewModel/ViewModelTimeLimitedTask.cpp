#include "UI/ViewModel/ViewModelTimeLimitedTask.h"

#include "Tasks/TimeLimitedQuestTask.h"
#include "UI/Widgets/TimeLimitedTaskWidget.h"
#include "QuestCoreTypes.h"
#include "QuestSubsystem.h"

UViewModelTimeLimitedTask::UViewModelTimeLimitedTask()
{
	TimeRefreshRate = 0.5f;
}

void UViewModelTimeLimitedTask::Initialize(UUserWidget* RelatedWidget)
{
	TimeLimitedTaskWidget = Cast<UTimeLimitedTaskWidget>(RelatedWidget);
	if (!IsValid(TimeLimitedTaskWidget))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("ViewModelTimeLimitedTask::Initialize: Faield to get TimeLimitedTaskWidget!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("ViewModelTimeLimitedTask::Initialize: Faield to get World!"));
		return;
	}

	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("ViewModelTimeLimitedTask::Initialize: QuestSubsystem is not valid!"));
		return;
	}

	TrackedTimeLimitedTask = Cast<UTimeLimitedQuestTask>(QuestSubsystem->GetTrackedQuestTask());
	if (!IsValid(TrackedTimeLimitedTask))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("ViewModelTimeLimitedTask::Initialize: QuestSubsystem is not valid!"));
		return;
	}

	UpdateTime();
	World->GetTimerManager().SetTimer(TimeRefreshHandler, FTimerDelegate::CreateUObject(this, &ThisClass::UpdateTime), TimeRefreshRate, true);
}

void UViewModelTimeLimitedTask::Deinitialize()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("ViewModelTimeLimitedTask::Deinitialize: Faield to get World!"));
		return;
	}

	World->GetTimerManager().ClearTimer(TimeRefreshHandler);
}

void UViewModelTimeLimitedTask::UpdateTime()
{
	if (IsValid(TimeLimitedTaskWidget) && IsValid(TrackedTimeLimitedTask))
	{
		TimeLimitedTaskWidget->SetRemainingTime(TrackedTimeLimitedTask->GetRemainingTime());
	}
}

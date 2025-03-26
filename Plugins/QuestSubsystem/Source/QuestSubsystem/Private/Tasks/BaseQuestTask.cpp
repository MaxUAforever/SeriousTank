#include "Tasks/BaseQuestTask.h"

#include "Tasks/TaskStartConditions/BaseTaskStartCondition.h"
#include "QuestCoreTypes.h"
#include "Quests/BaseQuest.h"

UBaseQuestTask::UBaseQuestTask(FTaskID InTaskID)
	: TaskID(InTaskID)
{
}

bool UBaseQuestTask::StartTask()
{
	if (!CanBeStarted())
	{
		return false;
	}

	bIsActive = true;
	OnQuestTaskStartedDelegate.Broadcast(TaskID);

	OnTaskStarted();

	return true;
}

bool UBaseQuestTask::FinishTask(EQuestCompleteRelust CompleteResult)
{
	if (!bIsActive)
	{
		return false;
	}

	bIsActive = false;
	bIsCompleted = CompleteResult == EQuestCompleteRelust::Succeeded;

	OnTaskCompleted(CompleteResult);
	OnQuestTaskCompletedDelegate.Broadcast(TaskID, CompleteResult);

	return true;
}

bool UBaseQuestTask::CanBeStarted() const
{
	if (bIsActive || bIsCompleted)
	{
		return false;
	}

	for (UBaseTaskStartCondition* StartCondition : StartConditions)
	{
		if (!IsValid(StartCondition))
		{
			continue;
		}

		if (!StartCondition->CanBeStarted())
		{
			return false;
		}
	}

	return true;
}

void UBaseQuestTask::AddStartCondition(UBaseTaskStartCondition* StartCondition)
{
	if (!IsValid(StartCondition))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UBaseQuestTask::AddStartCondition: StartCondition is not valid!"));
		return;
	}

	StartConditions.Add(StartCondition);
}

UObject* UBaseQuestTask::GetQuestProviderObject() const
{
	const UBaseQuest* Quest = Cast<UBaseQuest>(GetOuter());
	if (!IsValid(Quest))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UBaseQuestTask::GetQuestProviderObject: parent of task is not valid!"));
		return nullptr;
	}

	return Quest->GetQuestProviderObject();
}
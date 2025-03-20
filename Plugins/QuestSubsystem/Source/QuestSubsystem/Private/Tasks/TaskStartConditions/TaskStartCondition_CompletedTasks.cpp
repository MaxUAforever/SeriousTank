#include "Tasks/TaskStartConditions/TaskStartCondition_CompletedTasks.h"

#include "QuestSubsystem.h"

UTaskStartCondition_CompletedTasks::UTaskStartCondition_CompletedTasks()
{
	static_assert(std::is_same_v<FTaskID, int32>, "Element of CompletedTaskIDs should have a type of FTaskID!");
}

bool UTaskStartCondition_CompletedTasks::CanBeStarted()
{
	return true;
}
#pragma once

#include "BaseTaskStartCondition.h"
#include "QuestCoreTypes.h"
#include "TaskStartCondition_CompletedTasks.generated.h"

UCLASS()
class QUESTSUBSYSTEM_API UTaskStartCondition_CompletedTasks : public UBaseTaskStartCondition
{
    GENERATED_BODY()

public:
    UTaskStartCondition_CompletedTasks();

    virtual bool CanBeStarted() override;

    UFUNCTION()
    void SetCompletedTaskIDs(const TArray<int32>& InCompletedTaskIDs) { CompletedTaskIDs = InCompletedTaskIDs; };

private:
    UPROPERTY()
    TArray<int32> CompletedTaskIDs;
};

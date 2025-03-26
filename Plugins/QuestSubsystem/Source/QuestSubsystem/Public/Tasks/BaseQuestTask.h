#pragma once

#include "UObject/Object.h"
#include "QuestCoreTypes.h"
#include "BaseQuestTask.generated.h"

class UBaseTaskStartCondition;
class UQuestTaskInfoDataAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestTaskStartedDelegate, FTaskID);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuestTaskCompletedDelegate, FTaskID, EQuestCompleteRelust);

UCLASS(Abstract)
class QUESTSUBSYSTEM_API UBaseQuestTask : public UObject
{
	GENERATED_BODY()

public:
    FOnQuestTaskStartedDelegate OnQuestTaskStartedDelegate;
    FOnQuestTaskCompletedDelegate OnQuestTaskCompletedDelegate;

public:
    UBaseQuestTask() {};
    UBaseQuestTask(FTaskID InTaskID);

    bool StartTask();
    bool FinishTask(EQuestCompleteRelust CompleteResult);

    FTaskID GetID() const { return TaskID; };
    void SetID(FTaskID InTaskID) { TaskID = InTaskID; }

    bool IsActive() const { return bIsActive; };
    bool IsCompleted() const { return bIsCompleted; };

    void SetIsActive(bool bInIsActive) { bIsActive = bInIsActive; }
    void SetIsCompleted(bool bInIsCompleted) { bIsCompleted = bInIsCompleted; }

    bool CanBeStarted() const;

    FText GetTaskName() const { return TaskName; };
    FText GetTaskDescription() const { return TaskDescription; };

    void SetTaskName(const FText& InTaskName) { TaskName = InTaskName; };
    void SetTaskDescription(const FText& InTaskDescription) { TaskDescription = TaskDescription; };
    
    bool ShouldBeTracked() const { return bShouldBeTracked; }
    void SetShouldBeTracked(bool bInShouldBeTracked) { bShouldBeTracked = bInShouldBeTracked; }

    void AddStartCondition(UBaseTaskStartCondition* StartCondition);

    virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) {};
    virtual void PreSaveGame() {};

protected:
    virtual void OnTaskStarted() {};
    virtual void OnTaskCompleted(EQuestCompleteRelust CompleteResult) {};

    UObject* GetQuestProviderObject() const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TArray<TObjectPtr<UBaseTaskStartCondition>> StartConditions;

private:
    FTaskID TaskID = 0;

    FText TaskName;
    FText TaskDescription;
    bool bShouldBeTracked = true;

    bool bIsActive = false;
    bool bIsCompleted = false;
};

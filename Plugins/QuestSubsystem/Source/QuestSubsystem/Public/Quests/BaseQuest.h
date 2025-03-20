#pragma once

#include "UObject/Object.h"
#include "QuestCoreTypes.h"
#include "BaseQuest.generated.h"

enum class EQuestCompleteRelust : uint8
{
    Succeeded,
    Failed,
    Cancelled
};

class UBaseQuestTask;
class UQuestInfoDataAsset;

DECLARE_MULTICAST_DELEGATE(FOnQuestStartedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestCompletedDelegate, EQuestCompleteRelust);

UCLASS()
class QUESTSUBSYSTEM_API UBaseQuest : public UObject
{
    GENERATED_BODY()

public:
    FOnQuestStartedDelegate OnQuestStartedDelegate;
    FOnQuestCompletedDelegate OnQuestCompletedDelegate;

public:
    UBaseQuest() {};
    UBaseQuest(FQuestID QuestID);

    bool StartQuest();
    bool FinishQuest(EQuestCompleteRelust CompleteResult);

    FQuestID GetID() const { return QuestID; }
    void SetID(FQuestID InQuestID) { QuestID = InQuestID; }

    bool IsActive() const { return bIsActive; }
    bool IsCompleted() const { return bIsCompleted; }
    bool IsRepeatable() const { return bIsRepeatable; }

    void SetIsActive(bool bInIsActive) { bIsActive = bInIsActive; }
    void SetIsCompleted(bool bInIsCompleted) { bIsCompleted = bInIsCompleted; }
    void SetIsRepeatable(bool bInIsRepeatable) { bIsRepeatable = bInIsRepeatable; }

    FText GetQuestName() const { return QuestName; };
    FText GetQuestDescription() const { return QuestDescription; };

    void SetQuestName(const FText& InQuestName) { QuestName = InQuestName; };
    void SetQuestDescription(const FText& InQuestDescription) { QuestDescription = InQuestDescription; };

    const TArray<FTaskID>& GetTaskIDs() { return TaskIDsList; };
    void SetTasks(TArray<FTaskID>&& InTasks);
    void AddTask(FTaskID TaskID);

    virtual void FillQuestInfo(const UQuestInfoDataAsset* QuestInfoDataAsset) {};

    UObject* GetQuestProviderObject() const;

protected:
    virtual void OnQuestStarted() {};
    virtual void OnQuestFinished(EQuestCompleteRelust CompleteResult) {};

private:
    FQuestID QuestID = 0;
    TArray<FTaskID> TaskIDsList;

    FText QuestName;
    FText QuestDescription;

    bool bIsActive = false;
    bool bIsCompleted = false;
    bool bIsRepeatable = false;

    int32 CurrentTaskIndex = 0;
};

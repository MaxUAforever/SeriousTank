#include "Quests/BaseQuest.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "QuestCoreTypes.h"
#include "QuestSubsystem.h"

UBaseQuest::UBaseQuest(FQuestID InQuestID)
    : QuestID(InQuestID)
{
}

bool UBaseQuest::StartQuest()
{
    if (bIsActive || (bIsCompleted && !bIsRepeatable))
    {
        return false;
    }

    bIsActive = true;

    OnQuestStarted();
    OnQuestStartedDelegate.Broadcast();

    return true;
}

bool UBaseQuest::FinishQuest(EQuestCompleteRelust CompleteResult)
{
    if (!bIsActive)
    {
        return false;
    }

    bIsActive = false;
    bIsCompleted = (CompleteResult == EQuestCompleteRelust::Succeeded);

    OnQuestFinished(CompleteResult);
    OnQuestCompletedDelegate.Broadcast(CompleteResult);

    return true;
}

void UBaseQuest::SetTasks(TArray<FTaskID>&& InTasks)
{
    TaskIDsList = MoveTemp(InTasks);
}

void UBaseQuest::AddTask(FTaskID TaskID)
{
    TaskIDsList.AddUnique(TaskID);
}

UObject* UBaseQuest::GetQuestProviderObject() const
{
    UObject* Outer = GetOuter();
    if (!IsValid(Outer))
    {
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UBaseQuest::GetQuestProviderObject: parent of task is not valid!"));
		return GetWorld();
	}

    if (Outer->IsA<AGameModeBase>() || Outer->IsA<AGameStateBase>())
    {
        return GetWorld();
    }

    return Outer;
}

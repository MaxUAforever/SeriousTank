#include "Data/QuestSubsystemSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "QuestSubsystem.h"
#include "Utils/QuestSubsystemUtils.h"

FString UQuestSubsystemSaveGame::SaveSlotName = TEXT("QuestSubsystemSlot");
uint32 UQuestSubsystemSaveGame::UserIndex = 0;

void UQuestSubsystemSaveGame::AsyncLoadQuestsInfo(FQuestSubsystemAsyncLoadDelegate OnLoadedDelegate)
{
    auto OnAsyncLoadDone = [OnLoadedDelegate](const FString& /*SlotName*/, const int32 /*UserIndex*/, USaveGame* LoadedGameData)
    {
        UQuestSubsystemSaveGame* QuestSubsystemSaveGame = Cast<UQuestSubsystemSaveGame>(LoadedGameData);
        
		OnLoadedDelegate.ExecuteIfBound(IsValid(QuestSubsystemSaveGame) ? &QuestSubsystemSaveGame->QuestSubsystemSaveData : nullptr);
    };
    
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
    {
        OnLoadedDelegate.ExecuteIfBound(nullptr);
        return;
    }
    
    FAsyncLoadGameFromSlotDelegate LoadedDelegate = FAsyncLoadGameFromSlotDelegate::CreateLambda(OnAsyncLoadDone);
    UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, UserIndex, LoadedDelegate);
}

void UQuestSubsystemSaveGame::AsyncSaveQuestsInfo(const UQuestSubsystem* QuestSubsystem, FQuestSubsystemAsyncSaveDelegate OnSavedDelegate)
{
    USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(ThisClass::StaticClass());
    UQuestSubsystemSaveGame* SaveGameInstance = Cast<UQuestSubsystemSaveGame>(SaveGameObject);
    if (!SaveGameInstance || !IsValid(QuestSubsystem))
    {
        OnSavedDelegate.ExecuteIfBound(false);
    }

	StoreDataToSaveGame(QuestSubsystem, SaveGameInstance);
    
    auto OnAsyncSaveDone = [OnSavedDelegate](const FString& /*SlotName*/, const int32 /*UserIndex*/, bool bSuccess)
    {
        OnSavedDelegate.ExecuteIfBound(bSuccess);
    };
    
    FAsyncSaveGameToSlotDelegate SavedDelegate = FAsyncSaveGameToSlotDelegate::CreateLambda(OnAsyncSaveDone);
    UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex, SavedDelegate);
}

void UQuestSubsystemSaveGame::StoreDataToSaveGame(const UQuestSubsystem* QuestSubsystem, UQuestSubsystemSaveGame* SaveGameInstance)
{
	if (!SaveGameInstance || !QuestSubsystem)
	{
        return;
	}

	for (const auto& [QuestID, Quest] : QuestSubsystem->GetAllQuests())
	{
		if (!IsValid(Quest))
		{
			continue;
		}

        const FString AdditionalQuestInfo = QuestSubsystemUtils::CreateJSONStringForSaveGameParams(Quest);
		SaveGameInstance->QuestSubsystemSaveData.QuestsProgressList.Add({ QuestID, Quest->IsActive(), Quest->IsCompleted(), AdditionalQuestInfo });

		for (const FTaskID& TaskID : Quest->GetTaskIDs())
		{
			const UBaseQuestTask* Task = QuestSubsystem->GetQuestTask(TaskID);
			if (!IsValid(Task))
			{
				continue;
			}

            const FString AdditionalTaskInfo = QuestSubsystemUtils::CreateJSONStringForSaveGameParams(Task, Quest);
			SaveGameInstance->QuestSubsystemSaveData.QuestTasksProgressList.Add({ TaskID, QuestID, Task->IsActive(), Task->IsCompleted(), AdditionalTaskInfo });
		}
	}
}
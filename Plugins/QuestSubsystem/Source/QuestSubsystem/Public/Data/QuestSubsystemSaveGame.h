#pragma once

#include "GameFramework/SaveGame.h"
#include "QuestSubsystemSaveGame.generated.h"


DECLARE_DELEGATE_OneParam(FQuestSubsystemAsyncSaveDelegate, bool);
DECLARE_DELEGATE_OneParam(FQuestSubsystemAsyncLoadDelegate, const FQuestSubsystemSaveData*)

class UQuestSubsystem;

USTRUCT()
struct FQuestInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	int32 QuestID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	bool bIsActive = false;

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	bool bIsCompleted = false;
};

USTRUCT()
struct FQuestTaskInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	int32 TaskID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	int32 ParentQuestID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	bool bIsActive = false;

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	bool bIsCompleted = false;
};

USTRUCT()
struct FQuestSubsystemSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	TArray<FQuestInfo> QuestsProgressList;

	UPROPERTY(VisibleAnywhere, Category = "Quests")
	TArray<FQuestTaskInfo> QuestTasksProgressList;
};

UCLASS()
class QUESTSUBSYSTEM_API UQuestSubsystemSaveGame : public USaveGame
{
	GENERATED_BODY()
    
public:
    static void AsyncLoadQuestsInfo(FQuestSubsystemAsyncLoadDelegate OnLoadedDelegate = FQuestSubsystemAsyncLoadDelegate());
    static void AsyncSavedQuestsInfo(const UQuestSubsystem* QuestSubsystem, FQuestSubsystemAsyncSaveDelegate OnSavedDelegate = FQuestSubsystemAsyncSaveDelegate());

private:
	static void StoreDataToSaveGame(const UQuestSubsystem* QuestSubsystem, UQuestSubsystemSaveGame* SaveGameInstance);

private:
	static FString SaveSlotName;
	static uint32 UserIndex;

	UPROPERTY()
	FQuestSubsystemSaveData QuestSubsystemSaveData;
};

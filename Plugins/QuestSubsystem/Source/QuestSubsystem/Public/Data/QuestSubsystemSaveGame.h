#pragma once

#include "GameFramework/SaveGame.h"
#include "QuestSubsystemSaveGame.generated.h"


DECLARE_DELEGATE_OneParam(FQuestSubsystemAsyncSaveDelegate, bool);
DECLARE_DELEGATE_OneParam(FQuestSubsystemAsyncLoadDelegate, const FQuestSubsystemSaveData*)

class UQuestTaskAdditionalSaveInfo;
class UQuestSubsystem;

USTRUCT()
struct FQuestInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	int32 QuestID = 0;

	UPROPERTY(SaveGame)
	bool bIsActive = false;

	UPROPERTY(SaveGame)
	bool bIsCompleted = false;

	UPROPERTY(SaveGame)
	FString AdditionalQuestInfo;
};

USTRUCT()
struct FQuestTaskInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	int32 TaskID = 0;

	UPROPERTY(SaveGame)
	int32 ParentQuestID = 0;

	UPROPERTY(SaveGame)
	bool bIsActive = false;

	UPROPERTY(SaveGame)
	bool bIsCompleted = false;

	UPROPERTY(SaveGame)
	FString AdditionalTaskInfo;
};

USTRUCT()
struct FQuestSubsystemSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	TArray<FQuestInfo> QuestsProgressList;

	UPROPERTY(SaveGame)
	TArray<FQuestTaskInfo> QuestTasksProgressList;
};

UCLASS()
class QUESTSUBSYSTEM_API UQuestSubsystemSaveGame : public USaveGame
{
	GENERATED_BODY()
    
public:
    static void AsyncLoadQuestsInfo(FQuestSubsystemAsyncLoadDelegate OnLoadedDelegate = FQuestSubsystemAsyncLoadDelegate());
    static void AsyncSaveQuestsInfo(const UQuestSubsystem* QuestSubsystem, FQuestSubsystemAsyncSaveDelegate OnSavedDelegate = FQuestSubsystemAsyncSaveDelegate());

private:
	static void StoreDataToSaveGame(const UQuestSubsystem* QuestSubsystem, UQuestSubsystemSaveGame* SaveGameInstance);

private:
	static FString SaveSlotName;
	static uint32 UserIndex;

	UPROPERTY(SaveGame)
	FQuestSubsystemSaveData QuestSubsystemSaveData;
};

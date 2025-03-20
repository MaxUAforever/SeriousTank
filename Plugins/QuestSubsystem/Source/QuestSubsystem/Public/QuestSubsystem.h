#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/TimerHandle.h"
#include "Tasks/BaseQuestTask.h"
#include "Quests/BaseQuest.h"
#include "QuestCoreTypes.h"
#include "UI/Data/TableRowTaskWidgetInfo.h"
#include "QuestSubsystem.generated.h"


class UDataTable;
class UQuestProvider;
class UUserWidget;
class UViewModelBase;
struct FTableRowTaskStartConditionInfo;
enum class EQuestCompleteRelust : uint8;
enum class EQuestTaskCompleteResult : uint8;

DECLARE_MULTICAST_DELEGATE(FOnQuestsInitializedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrackedTaskChangedDelegate, TOptional<FTaskID>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTrackedTaskWidgetCreatedDelegate, UUserWidget*, UViewModelBase*);

/**
 * 
 */
UCLASS()
class QUESTSUBSYSTEM_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RegisterQuestProvider(UQuestProvider* QuestProvider);

	FOnQuestsInitializedDelegate& GetOnQuestsInitializedDelegate() { return OnQuestsInitializedDelegate; }
	FOnTrackedTaskChangedDelegate& GetOnTrackedTaskChangedDelegate() { return OnTrackedTaskChangedDelegate; }
	FOnTrackedTaskWidgetCreatedDelegate& GetOnTrackedTaskWidgetCreatedDelegate() { return OnTrackedTaskWidgetCreatedDelegate; };

	const UBaseQuest* GetQuest(FQuestID QuestID) const;
	const UBaseQuestTask* GetQuestTask(FTaskID TaskID) const;	

	const UBaseQuestTask* GetTrackedQuestTask() const;
	UBaseQuestTask* GetTrackedQuestTask();

	const TMap<int32, TObjectPtr<UBaseQuest>>& GetAllQuests() const { return CachedQuests; };
	const TMap<int32, TObjectPtr<UBaseQuestTask>>& GetAllTasks() const { return CachedTasks; };

	bool StartQuest(FQuestID QuestID);
	bool StartQuestTask(FTaskID TaskID);

	bool FinishQuest(FQuestID QuestID, EQuestCompleteRelust CompleteResult);
	bool FinishQuestTask(FTaskID TaskID, EQuestTaskCompleteResult CompleteResult);

	TOptional<FTaskID> GetTrackedTask() const { return TrackedTaskID; };
	bool SetTrackedTask(FTaskID TaskID);
	void ResetTrackedTask();

	UUserWidget* GetTrackedTaskWidget() { return TrackedTaskWidget; };

private:
	void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);
	void ResetAllData(bool bKeepSaveDataInfo);

	void RequestLoadDataTables();
	void OnQuestDataTablesLoaded();
	void UnloadDataTables();

	void FillQuestsForProvider(UQuestProvider* QuestProvider);
	void FillQuestsForRequestedProviders();
	void FillTaskStartConditions();
	void FillTaskWidgetsInfo();

	void RequestLoadSaveGame();

	void StartQuestProviderRegistration();

	UBaseQuest* AddQuest(FQuestID QuestID, TSubclassOf<UBaseQuest> BaseQuestClass, UQuestProvider* QuestProvider);
	UBaseQuestTask* AddQuestTask(FTaskID TaskID, FQuestID ParentQuestID, TSubclassOf<UBaseQuestTask> BaseTaskClass, UQuestProvider* QuestProvider);

	void CreateWidgetForTrackedTask();
	void OnTrackedTaskFinished(FTaskID TaskID, EQuestTaskCompleteResult TaskCompleteResult);

	UBaseQuest* GetQuest_Internal(FQuestID QuestID);
	UBaseQuestTask* GetQuestTask_Internal(FTaskID TaskID);

private:
	FOnQuestsInitializedDelegate OnQuestsInitializedDelegate;
	FOnTrackedTaskChangedDelegate OnTrackedTaskChangedDelegate;
	FOnTrackedTaskWidgetCreatedDelegate OnTrackedTaskWidgetCreatedDelegate;

	TSoftObjectPtr<UDataTable> QuestsInfoDataTable;
	TSoftObjectPtr<UDataTable> QuestTasksInfoDataTable;
	TSoftObjectPtr<UDataTable> TaskStartConditionsDataTable;
	TSoftObjectPtr<UDataTable> TaskWidgetsDataTable;

	/**
	 * Information about current quests registration and initializing
	 */
	bool bDataTablesLoaded = false;
	bool bQuestSaveGameLoaded = false;
	bool bIsQuestProviderRegisterActive = false;

	FTimerHandle RegistrationDelayTimer;
	TSet<UQuestProvider*> RequestedQuestProviders;

	/**
	 * Current tracked task properties.
	 */
	TOptional<FTaskID> TrackedTaskID;
	
	FDelegateHandle TrackedTaskFinishedDelegateHandle;

	UPROPERTY()
	TObjectPtr<UViewModelBase> TrackedTaskViewModel;

	UPROPERTY()
	TObjectPtr<UUserWidget> TrackedTaskWidget;

	/**
	 * Current stored quest and tasks with needed information
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UBaseQuest>> CachedQuests;

	UPROPERTY()
	TMap<int32, TObjectPtr<UBaseQuestTask>> CachedTasks;

	UPROPERTY()
	TMap<int32, FTableRowTaskWidgetInfo> CachedTaskWidgetInfo;
};

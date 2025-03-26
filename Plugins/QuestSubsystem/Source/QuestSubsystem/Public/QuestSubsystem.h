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
enum class EQuestCompleteRelust : uint8;

DECLARE_MULTICAST_DELEGATE(FOnQuestsInitializedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestsWorldCleanupDelegate, UWorld*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrackedTaskChangedDelegate, TOptional<FTaskID>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTrackedTaskWidgetCreatedDelegate, UUserWidget*, UViewModelBase*);

/**
 * @brief Subsystem that manages quests and quest tasks.
 *
 * This subsystem handles the registration, initialization, starting, and finishing of quests and their tasks. 
 * It also deals with loading required data tables, tracking tasks, and creating required widgets for tasks.
 */
UCLASS()
class QUESTSUBSYSTEM_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Initializes the quest subsystem.
	 * Sets up data tables, binds to world cleanup, and initializes default settings.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Registers a quest provider.
	 * Schedules registration (loading and initialization) of quests and tasks which IDs are presented in QuestProvider.
	 * By default this function is called by QuestProvider itself on BeginPlay.
	 * 
	 * @param QuestProvider - Pointer to the quest provider to register.
	 */
	void RegisterQuestProvider(UQuestProvider* QuestProvider);

	/**
	 * @brief Gets the delegate triggered when quests are initialized.
	 * @return Reference to the OnQuestsInitialized delegate.
	 */
	FOnQuestsInitializedDelegate& GetOnQuestsInitializedDelegate() { return OnQuestsInitializedDelegate; }
	
	/**
	 * @brief Gets the delegate triggered when World is cleanup, before saving and reseting information.
	 * @return Reference to the OnQuestsWorldCleanup delegate.
	 */
	FOnQuestsWorldCleanupDelegate& GetOnQuestsWorldCleanupDelegate() { return OnQuestsWorldCleanupDelegate;  }

	/**
	 * @brief Gets the delegate triggered when the tracked task changes.
	 * @return Reference to the OnTrackedTaskChanged delegate.
	 */
	FOnTrackedTaskChangedDelegate& GetOnTrackedTaskChangedDelegate() { return OnTrackedTaskChangedDelegate; }
	
	/**
	 * @brief Gets the delegate triggered when a widget for the tracked task is created.
	 * @return Reference to the OnTrackedTaskWidgetCreated delegate.
	 */
	FOnTrackedTaskWidgetCreatedDelegate& GetOnTrackedTaskWidgetCreatedDelegate() { return OnTrackedTaskWidgetCreatedDelegate; };

	/**
	 * @brief Retrieves a quest by its ID.
	 *
	 * @param QuestID The ID of the requested quest.
	 * @return Pointer to the quest or nullptr if not found.
	 */
	const UBaseQuest* GetQuest(FQuestID QuestID) const;
	const UBaseQuestTask* GetQuestTask(FTaskID TaskID) const;	

	/**
	 * @brief Gets the currently tracked quest task.
	 * @return Pointer to the tracked quest task or nullptr if no task is tracked.
	 */
	const UBaseQuestTask* GetTrackedQuestTask() const;
	UBaseQuestTask* GetTrackedQuestTask();

	/**
	 * @brief Gets all cached quests.
	 * @return Constant reference to the map of all quests.
	 */
	const TMap<int32, TObjectPtr<UBaseQuest>>& GetAllQuests() const { return CachedQuests; };
	
	/**
	 * @brief Gets all cached quest tasks.
	 * @return Constant reference to the map of all tasks.
	 */
	const TMap<int32, TObjectPtr<UBaseQuestTask>>& GetAllTasks() const { return CachedTasks; };

	/**
	 * @brief Starts the quest with the specified ID.
	 * Starts the quest and automatically starts all associated tasks.
	 *
	 * @param QuestID The ID of the quest to start.
	 * @param bShouldStartTasks Determines if tasks that can be started should start with parent quest.
	 * @return True if the quest started successfully; false otherwise.
	 */
	bool StartQuest(FQuestID QuestID, bool bShouldStartTasks = false);

	/**
	 * @brief Starts a specific quest task.
	 * Begins the specified quest task and sets it as tracked if conditions are met.
	 *
	 * @param TaskID The ID of the task to start.
	 * @return True if the task started successfully; false otherwise.
	 */
	bool StartQuestTask(FTaskID TaskID);

	/**
	 * @brief Finishes a quest with a given result.
	 * Marks the quest as finished with the specified result and finishes all associated tasks accordingly.
	 *
	 * @param QuestID The ID of the quest to finish.
	 * @param CompleteResult The result of quest completion.
	 * @return True if the quest finished successfully; false otherwise.
	 */
	bool FinishQuest(FQuestID QuestID, EQuestCompleteRelust CompleteResult);
	
	/**
	 * @brief Finishes a quest task with a given result.
	 * Marks the specified quest task as finished.
	 *
	 * @param TaskID The ID of the task to finish.
	 * @param CompleteResult The result of task completion.
	 * @return True if the task finished successfully; false otherwise.
	 */
	bool FinishQuestTask(FTaskID TaskID, EQuestCompleteRelust CompleteResult);

	/**
	 * @brief Gets the currently tracked task ID.
	 * 
	 * @return An optional containing the tracked task ID, or empty if no task is tracked.
	 */
	TOptional<FTaskID> GetTrackedTask() const { return TrackedTaskID; };

	/**
	 * @brief Sets the task to be tracked.
	 * Updates the tracked task if the specified task is valid and active.
	 *
	 * @param TaskID The ID of the task to track.
	 * @return True if the tracked task was updated successfully; false otherwise.
	 */
	bool SetTrackedTask(FTaskID TaskID);

	/**
	 * @brief Resets the tracked task.
	 * Clears the current tracked task and notifies listeners.
	 */
	void ResetTrackedTask();

	/**
	 * @brief Retrieves the widget for the currently tracked task.
	 *
	 * @return Pointer to the user widget associated with the tracked task.
	 */
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

	bool HasOnlyCompletedTasks(const UBaseQuest* Quest) const;
	void OnQuestTaskCompleted(FTaskID TaskID, EQuestCompleteRelust TaskCompleteResult);
	void SaveQuestsState();

	UBaseQuest* GetQuest_Internal(FQuestID QuestID);
	UBaseQuestTask* GetQuestTask_Internal(FTaskID TaskID);

	UBaseQuest* FindParentQuest(FTaskID TaskID);

private:
	FOnQuestsInitializedDelegate OnQuestsInitializedDelegate;
	FOnQuestsWorldCleanupDelegate OnQuestsWorldCleanupDelegate;
	FOnTrackedTaskChangedDelegate OnTrackedTaskChangedDelegate;
	FOnTrackedTaskWidgetCreatedDelegate OnTrackedTaskWidgetCreatedDelegate;

	TSoftObjectPtr<UDataTable> QuestsInfoDataTable;
	TSoftObjectPtr<UDataTable> QuestTasksInfoDataTable;
	TSoftObjectPtr<UDataTable> TaskStartConditionsDataTable;
	TSoftObjectPtr<UDataTable> TaskWidgetsDataTable;

	/**
	 * @brief Flags indicating the status of data loading and quest initialization.
	 */
	bool bDataTablesLoaded = false;
	bool bQuestSaveGameLoaded = false;
	bool bIsQuestProviderRegisterActive = false;

	FTimerHandle RegistrationDelayTimer;
	TSet<UQuestProvider*> RequestedQuestProviders;

	/**
	 *  @brief Currently tracked task properties.
	 */
	TOptional<FTaskID> TrackedTaskID;

	UPROPERTY()
	TObjectPtr<UViewModelBase> TrackedTaskViewModel;

	UPROPERTY()
	TObjectPtr<UUserWidget> TrackedTaskWidget;

	/**
	 *  @brief Currently stored quest and tasks with needed information
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UBaseQuest>> CachedQuests;

	UPROPERTY()
	TMap<int32, TObjectPtr<UBaseQuestTask>> CachedTasks;

	UPROPERTY()
	TMap<int32, FTableRowTaskWidgetInfo> CachedTaskWidgetInfo;

	TMap<FTaskID, FDelegateHandle> TaskFinishedDelegateHandles;
};

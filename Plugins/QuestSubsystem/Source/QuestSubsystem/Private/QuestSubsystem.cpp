#include "QuestSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Components/QuestProvider.h"
#include "Data/Quests/TableRowQuestInfo.h"
#include "Data/Tasks/TableRowQuestTaskInfo.h"
#include "Data/Tasks/TasksStartConditions/TableRowTaskStartConditionInfo.h"
#include "Data/Tasks/QuestTaskInfoDataAsset.h"
#include "Data/Quests/QuestInfoDataAsset.h"
#include "Data/QuestSubsystemSaveGame.h"
#include "Data/QuestSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Tasks/TaskStartConditions/BaseTaskStartCondition.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "Utils/QuestSubsystemUtils.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UQuestSubsystemSettings* QuestSettings = GetDefault<UQuestSubsystemSettings>();
	QuestsInfoDataTable = QuestSettings->QuestsDataTable;
	QuestTasksInfoDataTable = QuestSettings->QuestTasksDataTable;
	TaskStartConditionsDataTable = QuestSettings->TaskStartConditionsDataTable;

	if (QuestSettings->bShouldTaskUseWidgets)
	{
		TaskWidgetsDataTable = QuestSettings->TaskWidgetsDataTable;
	}

	FWorldDelegates::OnWorldCleanup.AddUObject(this, &ThisClass::OnWorldCleanup);
}

const UBaseQuest* UQuestSubsystem::GetQuest(FQuestID QuestID) const
{
	const TObjectPtr<UBaseQuest>* Quest = CachedQuests.Find(QuestID);

	return Quest != nullptr ? *Quest : nullptr;
}

const UBaseQuestTask* UQuestSubsystem::GetQuestTask(FTaskID TaskID) const
{
	const TObjectPtr<UBaseQuestTask>* QuestTask = CachedTasks.Find(TaskID);
	
	return QuestTask != nullptr ? *QuestTask : nullptr;
}

const UBaseQuestTask* UQuestSubsystem::GetTrackedQuestTask() const
{
	if (!TrackedTaskID.IsSet())
	{
		return nullptr;
	}

	return GetQuestTask(TrackedTaskID.GetValue());
}

UBaseQuestTask* UQuestSubsystem::GetTrackedQuestTask()
{
	if (!TrackedTaskID.IsSet())
	{
		return nullptr;
	}

	return GetQuestTask_Internal(TrackedTaskID.GetValue());
}

bool UQuestSubsystem::StartQuest(FQuestID QuestID, bool bShouldStartTasks)
{
	TObjectPtr<UBaseQuest> Quest = GetQuest_Internal(QuestID);
	if (!IsValid(Quest))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::StartQuest: Quest with ID %d is not found!"), QuestID);
		return false;
	}

	bool bSuccess = Quest->StartQuest();
	if (!bSuccess)
	{
		return false;
	}

	const bool bIsTasksStateResetNeeded = Quest->IsRepeatable() && Quest->IsCompleted();
	if (!bShouldStartTasks && !bIsTasksStateResetNeeded)
	{
		return bSuccess;
	}

	for (FTaskID TaskID : Quest->GetTaskIDs())
	{
		if (bIsTasksStateResetNeeded)
		{
			if (UBaseQuestTask* QuestTask = GetQuestTask_Internal(TaskID))
			{
				QuestTask->SetIsCompleted(false);
			}
		}
		
		if (bShouldStartTasks)
		{
			StartQuestTask(TaskID);
		}
	}

	return bSuccess;
}

bool UQuestSubsystem::StartQuestTask(FTaskID TaskID)
{
	TObjectPtr<UBaseQuestTask> QuestTask = GetQuestTask_Internal(TaskID);
	if (!IsValid(QuestTask))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::StartQuestTask: Task with ID %d is not found!"), TaskID);
		return false;
	}

	const bool bSuccess = QuestTask->StartTask();
	if (bSuccess)
	{
		QuestTask->OnQuestTaskCompletedDelegate.AddUObject(this, &ThisClass::OnQuestTaskCompleted);
		
		const UQuestSubsystemSettings* QuestSettings = GetDefault<UQuestSubsystemSettings>();
		if (QuestSettings->bShouldTrackNewActiveTasks && QuestTask->ShouldBeTracked())
		{
			SetTrackedTask(TaskID);
		}
	}

	return bSuccess;
}

bool UQuestSubsystem::FinishQuest(FQuestID QuestID, EQuestCompleteRelust CompleteResult)
{
	TObjectPtr<UBaseQuest> Quest = GetQuest_Internal(QuestID);
	if (!IsValid(Quest))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::FinishQuest: Quest with ID %d is not found!"), QuestID);
		return false;
	}

	bool bSuccess = Quest->FinishQuest(CompleteResult);
	if (bSuccess)
	{
		for (FTaskID TaskID : Quest->GetTaskIDs())
		{
			FinishQuestTask(TaskID, CompleteResult);
		}
	}

	return bSuccess;
}

bool UQuestSubsystem::FinishQuestTask(FTaskID TaskID, EQuestCompleteRelust CompleteResult)
{
	TObjectPtr<UBaseQuestTask> QuestTask = GetQuestTask_Internal(TaskID);
	if (!IsValid(QuestTask))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::FinishQuestTask: Task with ID %d is not found!"), TaskID);
		return false;
	}

	return QuestTask->FinishTask(CompleteResult);
}

bool UQuestSubsystem::SetTrackedTask(FTaskID TaskID)
{
	UBaseQuestTask* Task = GetQuestTask_Internal(TaskID);
	if (!IsValid(Task) || !Task->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::SetTrackedTask: Task with ID %d is not valid or not active!"), TaskID);
		return false;
	}

	if (TrackedTaskID.IsSet())
	{
		ResetTrackedTask();
	}

	TrackedTaskID = TaskID;

	const UQuestSubsystemSettings* QuestSettings = GetDefault<UQuestSubsystemSettings>();
	if (QuestSettings->bShouldTaskUseWidgets)
	{
		CreateWidgetForTrackedTask();
	}

	OnTrackedTaskChangedDelegate.Broadcast(TrackedTaskID);

	return true;
}

void UQuestSubsystem::ResetTrackedTask()
{
	if (!TrackedTaskID.IsSet())
	{
		return;
	}

	if (IsValid(TrackedTaskViewModel))
	{
		TrackedTaskViewModel->Deinitialize();
		TrackedTaskViewModel = nullptr;
	}

	TrackedTaskID.Reset();

	const UQuestSubsystemSettings* QuestSettings = GetDefault<UQuestSubsystemSettings>();
	if (QuestSettings->bShouldTaskUseWidgets)
	{
		OnTrackedTaskWidgetCreatedDelegate.Broadcast(nullptr, nullptr);
	}
}

void UQuestSubsystem::RegisterQuestProvider(UQuestProvider* QuestProvider)
{
	RequestedQuestProviders.Add(QuestProvider);

	if (!bIsQuestProviderRegisterActive)
	{
		// Handle requested quests and tasks with delay to prevent separate handling for each QuestProvider
		GetWorld()->GetTimerManager().SetTimer(RegistrationDelayTimer, FTimerDelegate::CreateUObject(this, &ThisClass::StartQuestProviderRegistration), 2.f, false);
	}
}

void UQuestSubsystem::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	bIsQuestProviderRegisterActive = false;
	
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(RegistrationDelayTimer);
	}

	OnQuestsWorldCleanupDelegate.Broadcast(World);

	SaveQuestsState();

	ResetTrackedTask();
	ResetAllData(true);

	RequestedQuestProviders.Empty();
}

void UQuestSubsystem::ResetAllData(bool bKeepSaveDataInfo)
{
	bQuestSaveGameLoaded = bKeepSaveDataInfo;
	
	if (bKeepSaveDataInfo)
	{
		for (auto QuestIt = CachedQuests.CreateIterator(); QuestIt; ++QuestIt)
		{
			const UBaseQuest* BaseQuest = QuestIt.Value();
			if (!IsValid(BaseQuest) || (!BaseQuest->IsActive() && !BaseQuest->IsCompleted()))
			{
				QuestIt.RemoveCurrent();
			}
		}

		for (auto TaskIt = CachedTasks.CreateIterator(); TaskIt; ++TaskIt)
		{
			const UBaseQuestTask* BaseTask = TaskIt.Value();
			if (!IsValid(BaseTask))
			{
				TaskIt.RemoveCurrent();
			}
			else if (!BaseTask->IsActive() && !BaseTask->IsCompleted())
			{
				CachedTaskWidgetInfo.Remove(BaseTask->GetID());
				TaskIt.RemoveCurrent();
			}
		}
	}
	else
	{
		CachedQuests.Empty();
		CachedTasks.Empty();
		CachedTaskWidgetInfo.Empty();
	}
}

void UQuestSubsystem::RequestLoadDataTables()
{
	TArray<FSoftObjectPath> NeededDataTablePathes;
	if (QuestsInfoDataTable.IsPending())
	{
		NeededDataTablePathes.Add(QuestsInfoDataTable.ToSoftObjectPath());
	}

	if (QuestTasksInfoDataTable.IsPending())
	{
		NeededDataTablePathes.Add(QuestTasksInfoDataTable.ToSoftObjectPath());
	}

	if (TaskStartConditionsDataTable.IsPending())
	{
		NeededDataTablePathes.Add(TaskStartConditionsDataTable.ToSoftObjectPath());
	}

	if (TaskWidgetsDataTable.IsPending())
	{
		NeededDataTablePathes.Add(TaskWidgetsDataTable.ToSoftObjectPath());
	}

	if (!NeededDataTablePathes.IsEmpty())
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(NeededDataTablePathes, FStreamableDelegate::CreateUObject(this, &UQuestSubsystem::OnQuestDataTablesLoaded));
	}
	else
	{
		OnQuestDataTablesLoaded();
	}
}

void UQuestSubsystem::OnQuestDataTablesLoaded()
{
	if (!QuestsInfoDataTable.IsValid() || !QuestTasksInfoDataTable.IsValid() || !TaskStartConditionsDataTable.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::OnQuestDataTablesLoaded: Failed to load Data tables!"));
		return;
	}

	const bool bIsValidQuestsRowStruct = QuestsInfoDataTable->GetRowStruct()->IsChildOf(FTableRowQuestInfo::StaticStruct());
	const bool bIsValidTasksRowStruct = QuestTasksInfoDataTable->GetRowStruct()->IsChildOf(FTableRowQuestTaskInfo::StaticStruct());
	const bool bIsValidStartConditionsRowStruct = TaskStartConditionsDataTable->GetRowStruct()->IsChildOf(FTableRowTaskStartConditionInfo::StaticStruct());
	const bool bIsValidTaskWidgetsRowStruct = TaskWidgetsDataTable.IsValid() ? TaskWidgetsDataTable->GetRowStruct()->IsChildOf(FTableRowTaskWidgetInfo::StaticStruct()) : true;

	if (!bIsValidQuestsRowStruct || !bIsValidTasksRowStruct || !bIsValidStartConditionsRowStruct || !bIsValidTaskWidgetsRowStruct)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::OnQuestDataTablesLoaded: RowStruct of one of Data tables is incorrect!"));
		return;
	}

	bDataTablesLoaded = true;
	if (bIsQuestProviderRegisterActive)
	{
		StartQuestProviderRegistration();
	}
}

void UQuestSubsystem::UnloadDataTables()
{
	bDataTablesLoaded = false;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.Unload(QuestsInfoDataTable.ToSoftObjectPath());
	Streamable.Unload(QuestTasksInfoDataTable.ToSoftObjectPath());
	Streamable.Unload(TaskStartConditionsDataTable.ToSoftObjectPath());
	Streamable.Unload(TaskWidgetsDataTable.ToSoftObjectPath());
}

void UQuestSubsystem::FillQuestsForProvider(UQuestProvider* QuestProvider)
{
	for (const FQuestID& QuestID : QuestProvider->GetQuestIDs())
	{
		const FTableRowQuestInfo* QuestInfo = QuestsInfoDataTable->FindRow<FTableRowQuestInfo>(FName(FString::FromInt(QuestID)), TEXT("Finding Quest Info Row"));
		if (!QuestInfo)
		{
			continue;
		}

		UBaseQuest* BaseQuest = AddQuest(QuestID, QuestInfo->BaseQuestClass, QuestProvider);
		if (!IsValid(BaseQuest))
		{
			continue;
		}

		BaseQuest->SetQuestName(QuestInfo->QuestName);
		BaseQuest->SetQuestDescription(QuestInfo->QuestDescription);
		BaseQuest->SetIsRepeatable(QuestInfo->bIsRepeatable);

		if (IsValid(QuestInfo->QuestInfoDataAsset))
		{
			BaseQuest->FillQuestInfo(QuestInfo->QuestInfoDataAsset);
		}
	}

	for (const FTaskID& TaskID : QuestProvider->GetQuestTaskIDs())
	{
		const FTableRowQuestTaskInfo* TaskInfo = QuestTasksInfoDataTable->FindRow<FTableRowQuestTaskInfo>(FName(FString::FromInt(TaskID)), TEXT("Finding Quest Task Info Row"));
		if (!TaskInfo)
		{
			return;
		}

		UBaseQuestTask* BaseTask = AddQuestTask(TaskID, TaskInfo->ParentQuestID, TaskInfo->BaseQuestTaskClass, QuestProvider);
		if (!IsValid(BaseTask))
		{
			continue;
		}

		BaseTask->SetTaskName(TaskInfo->TaskName);
		BaseTask->SetTaskDescription(TaskInfo->TaskDescription);
		BaseTask->SetShouldBeTracked(TaskInfo->bShouldBeTracked);

		if (IsValid(TaskInfo->QuestTaskInfoDataAsset))
		{
			BaseTask->FillTaskInfo(TaskInfo->QuestTaskInfoDataAsset);
		}
	}
}

void UQuestSubsystem::FillQuestsForRequestedProviders()
{
	for (UQuestProvider* QuestProvider : RequestedQuestProviders)
	{
		FillQuestsForProvider(QuestProvider);
	}
}

void UQuestSubsystem::FillTaskStartConditions()
{
	auto FillTaskStartConditionsLambda = [this](const FName& Key, const FTableRowTaskStartConditionInfo& TaskStartConditionInfo)
	{
		UBaseQuestTask* BaseTask = GetQuestTask_Internal(TaskStartConditionInfo.ParentTaskID);
		if (!IsValid(BaseTask))
		{
			return;
		}

		UBaseTaskStartCondition* BaseTaskStartCondition = NewObject<UBaseTaskStartCondition>(BaseTask, TaskStartConditionInfo.BaseTaskStartConditionClass);
		if (!IsValid(BaseTaskStartCondition))
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::OnQuestDataTablesLoaded: Failed to create BaseTaskStartCondition for TaskID %d!"), TaskStartConditionInfo.ParentTaskID);
			return;
		}

		QuestSubsystemUtils::FillObjectParamsFromJSONString(BaseTaskStartCondition, TaskStartConditionInfo.ConditionParametersJSON);
		BaseTask->AddStartCondition(BaseTaskStartCondition);
	};

	TaskStartConditionsDataTable->ForeachRow<FTableRowTaskStartConditionInfo>(TEXT("Reading QuestTasksInfoDataTable"), FillTaskStartConditionsLambda);
}

void UQuestSubsystem::FillTaskWidgetsInfo()
{
	auto FillTaskWidgetLambda = [this](const FName& Key, const FTableRowTaskWidgetInfo& WidgetInfo)
	{
		const FTaskID TaskID = FCString::Atoi(*Key.ToString());
		CachedTaskWidgetInfo.Add(TaskID, WidgetInfo);
	};

	if (TaskWidgetsDataTable.IsValid())
	{
		TaskWidgetsDataTable->ForeachRow<FTableRowTaskWidgetInfo>(TEXT("Reading TaskWidgetsDataTable"), FillTaskWidgetLambda);
	}
}

void UQuestSubsystem::RequestLoadSaveGame()
{
	auto OnSaveGameLoaded = [this](const FQuestSubsystemSaveData* QuestSubsystemSaveData)
	{
		bQuestSaveGameLoaded = true;

		if (QuestSubsystemSaveData == nullptr)
		{
			if (bIsQuestProviderRegisterActive)
			{
				StartQuestProviderRegistration();
			}

			return;
		}

		for (const FQuestInfo& QuestInfo : QuestSubsystemSaveData->QuestsProgressList)
		{
			if (!CachedQuests.Contains(QuestInfo.QuestID))
			{
				continue;
			}
				
			UBaseQuest* BaseQuest = GetQuest_Internal(QuestInfo.QuestID);
			if (!IsValid(BaseQuest))
			{
				continue;
			}

			BaseQuest->SetIsCompleted(QuestInfo.bIsCompleted);
			QuestSubsystemUtils::FillObjectParamsFromJSONString(BaseQuest, QuestInfo.AdditionalQuestInfo);

			if (QuestInfo.bIsActive)
			{
				StartQuest(QuestInfo.QuestID);
			}
		}

		for (const FQuestTaskInfo& TaskInfo : QuestSubsystemSaveData->QuestTasksProgressList)
		{
			if (!CachedTasks.Contains(TaskInfo.TaskID))
			{
				continue;
			}

			UBaseQuestTask* BaseTask = GetQuestTask_Internal(TaskInfo.TaskID);
			if (!IsValid(BaseTask))
			{
				continue;
			}

			BaseTask->SetIsCompleted(TaskInfo.bIsCompleted);
			QuestSubsystemUtils::FillObjectParamsFromJSONString(BaseTask, TaskInfo.AdditionalTaskInfo);
			
			if (TaskInfo.bIsActive)
			{
				StartQuestTask(TaskInfo.TaskID);
			}
		}

		if (bIsQuestProviderRegisterActive)
		{
			StartQuestProviderRegistration();
		}
	};

	UQuestSubsystemSaveGame::AsyncLoadQuestsInfo(FQuestSubsystemAsyncLoadDelegate::CreateLambda(OnSaveGameLoaded));
}

UBaseQuest* UQuestSubsystem::AddQuest(FQuestID QuestID, TSubclassOf<UBaseQuest> BaseQuestClass, UQuestProvider* QuestProvider)
{
	if (!IsValid(QuestProvider))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::AddQuest: QuestProvider is not valid!"));
		return nullptr;
	}

	const bool bIsNewQuest = !CachedQuests.Contains(QuestID);

	UBaseQuest* BaseQuest = !bIsNewQuest ? GetQuest_Internal(QuestID) : nullptr;
	if (!IsValid(BaseQuest))
	{
		BaseQuest = CachedQuests.Add(QuestID, NewObject<UBaseQuest>(QuestProvider->GetOuter(), BaseQuestClass));
		BaseQuest->SetID(QuestID);
	}

	return BaseQuest;
}

UBaseQuestTask* UQuestSubsystem::AddQuestTask(FTaskID TaskID, FQuestID ParentQuestID, TSubclassOf<UBaseQuestTask> BaseTaskClass, UQuestProvider* QuestProvider)
{
	if (!IsValid(QuestProvider))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::AddQuestTask: QuestProvider is not valid!"));
		return nullptr;
	}

	if (!IsValid(BaseTaskClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem::AddQuestTask: BaseTaskClass is not valid!"));
		return nullptr;
	}

	TObjectPtr<UBaseQuest> ParentQuest = GetQuest_Internal(ParentQuestID);
	if (!IsValid(ParentQuest))
	{
		return nullptr;
	}

	const bool bIsNewTask = !CachedTasks.Contains(TaskID);

	UBaseQuestTask* BaseTask = !bIsNewTask ? GetQuestTask_Internal(TaskID) : nullptr;
	if (!IsValid(BaseTask))
	{
		BaseTask = CachedTasks.Add(TaskID, NewObject<UBaseQuestTask>(ParentQuest, BaseTaskClass));
		BaseTask->SetID(TaskID);

		ParentQuest->AddTask(TaskID);
	}

	return BaseTask;
}

void UQuestSubsystem::StartQuestProviderRegistration()
{
	bIsQuestProviderRegisterActive = true;

	if (!bDataTablesLoaded)
	{
		RequestLoadDataTables();
		return;
	}

	FillQuestsForRequestedProviders();
	FillTaskStartConditions();
	FillTaskWidgetsInfo();

	if (!bQuestSaveGameLoaded)
	{
		RequestLoadSaveGame();
		return;
	}

	bIsQuestProviderRegisterActive = false;

	OnQuestsInitializedDelegate.Broadcast();

	RequestedQuestProviders.Empty();
	UnloadDataTables();
}

void UQuestSubsystem::CreateWidgetForTrackedTask()
{
	if (!TrackedTaskID.IsSet())
	{
		return;
	}

	const FTableRowTaskWidgetInfo* TaskWidgetInfo = CachedTaskWidgetInfo.Find(TrackedTaskID.GetValue());
	if (TaskWidgetInfo == nullptr)
	{
		return;
	}

	TrackedTaskWidget = CreateWidget<UUserWidget>(GetGameInstance(), TaskWidgetInfo->QuestTaskWidgetClass);
	if (!IsValid(TrackedTaskWidget))
	{
		return;
	}

	if (IsValid(TrackedTaskViewModel))
	{
		TrackedTaskViewModel->Deinitialize();
	}

	if (IsValid(TaskWidgetInfo->QuestTaskViewModelClass))
	{
		TrackedTaskViewModel = NewObject<UViewModelBase>(this, TaskWidgetInfo->QuestTaskViewModelClass);
		if (!IsValid(TrackedTaskViewModel))
		{
			return;
		}

		TrackedTaskViewModel->Initialize(TrackedTaskWidget);
	}

	OnTrackedTaskWidgetCreatedDelegate.Broadcast(TrackedTaskWidget, TrackedTaskViewModel);
}

bool UQuestSubsystem::HasOnlyCompletedTasks(const UBaseQuest* Quest) const
{
	if (!IsValid(Quest))
	{
		return false;
	}

	for (FTaskID ChildTaskID : Quest->GetTaskIDs())
	{
		const UBaseQuestTask* Task = GetQuestTask(ChildTaskID);
		if (!IsValid(Task))
		{
			continue;
		}

		if (!Task->IsCompleted())
		{
			return false;
		}
	}

	return true;
}

void UQuestSubsystem::OnQuestTaskCompleted(FTaskID TaskID, EQuestCompleteRelust TaskCompleteResult)
{
	UBaseQuestTask* BaseTask = GetQuestTask_Internal(TaskID);
	if (!IsValid(BaseTask))
	{
		return;
	}

	FDelegateHandle* DelegateHandle = TaskFinishedDelegateHandles.Find(TaskID);
	if (DelegateHandle != nullptr)
	{
		BaseTask->OnQuestTaskCompletedDelegate.Remove(*DelegateHandle);
		TaskFinishedDelegateHandles.Remove(TaskID);
	}

	UBaseQuest* ParentQuest = FindParentQuest(TaskID);
	if (IsValid(ParentQuest) && HasOnlyCompletedTasks(ParentQuest))
	{
		ParentQuest->FinishQuest(TaskCompleteResult);
	}
	
	if (TrackedTaskID == TaskID)
	{
		ResetTrackedTask();
	}

	SaveQuestsState();
}

void UQuestSubsystem::SaveQuestsState()
{
	for (const auto& [QuestID, Quest] : CachedQuests)
	{
		if (IsValid(Quest) && Quest->IsActive())
		{
			Quest->PreSaveGame();
		}
	}
	for (const auto& [QuestID, Task] : CachedTasks)
	{
		if (IsValid(Task) && Task->IsActive())
		{
			Task->PreSaveGame();
		}
	}

	UQuestSubsystemSaveGame::AsyncSaveQuestsInfo(this);
}

UBaseQuest* UQuestSubsystem::GetQuest_Internal(FQuestID QuestID)
{
	const TObjectPtr<UBaseQuest>* Quest = CachedQuests.Find(QuestID);

	return Quest != nullptr ? *Quest : nullptr;
}

UBaseQuestTask* UQuestSubsystem::GetQuestTask_Internal(FTaskID TaskID)
{
	const TObjectPtr<UBaseQuestTask>* QuestTask = CachedTasks.Find(TaskID);

	return QuestTask != nullptr ? *QuestTask : nullptr;
}

UBaseQuest* UQuestSubsystem::FindParentQuest(FTaskID InTaskID)
{
	for (auto& [QuestID, Quest] : CachedQuests)
	{
		for (FTaskID TaskID : Quest->GetTaskIDs())
		{
			if (InTaskID == TaskID)
			{
				return Quest;
			}
		}
	}

	return nullptr;
}

#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_AIAreaClearing.h"

#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"
#include "Subsystems/QuestSubsystem/Tasks/Data/ST_AIAreaClearingTaskInfoDataAsset.h"

void UST_QuestTask_AIAreaClearing::FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset)
{
	const UST_AIAreaClearingTaskInfoDataAsset* AreaClearingDataAsset = Cast<UST_AIAreaClearingTaskInfoDataAsset>(QuestInfoDataAsset);
	if (!IsValid(AreaClearingDataAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::FillQuestInfo: AIAreaClearingTaskInfoDataAsset is not valid!"));
		return;
	}

	EnemiesWavesAIAmount.Empty();
	EnemiesWavesAIAmount.Append(AreaClearingDataAsset->EnemiesWavesAIAmount);
}

void UST_QuestTask_AIAreaClearing::OnTaskStarted()
{
	CurrentWaveIndex = 0;
	CurrentWaveAIAmount = 0;

	if (EnemiesWavesAIAmount.IsEmpty())
	{
		return;
	}

	UObjectSpawnSubsystem* ObjectSpawnSubsystem = GetWorld()->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(ObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::OnTaskStarted: ObjectSpawnSubsystem is nullptr!"));
		return;
	}

	ObjectSpawnSubsystem->FindOrAddObjectSpawnManager(ESpawnObjectType::AIPawn, GetQuestProviderObject());
	FOnObjectSpawnedDelegate* OnObjectSpawnedDelegate = ObjectSpawnSubsystem->GetManagerObjectSpawnedDelegate(ESpawnObjectType::AIPawn, GetQuestProviderObject());
	if (OnObjectSpawnedDelegate == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::OnTaskStarted: OnObjectSpawnedDelegate is nullptr!"));
		return;
	}

	OnObjectSpawnedDelegateHandle = OnObjectSpawnedDelegate->AddUObject(this, &ThisClass::OnEnemySpawned);

	StartWave(CurrentWaveIndex);
}

void UST_QuestTask_AIAreaClearing::OnTaskCompleted(EQuestTaskCompleteResult CompleteResult)
{
	UObjectSpawnSubsystem* ObjectSpawnSubsystem = GetWorld()->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(ObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::OnTaskCompleted: ObjectSpawnSubsystem is nullptr!"));
		return;
	}

	FOnObjectSpawnedDelegate* OnObjectSpawnedDelegate = ObjectSpawnSubsystem->GetManagerObjectSpawnedDelegate(ESpawnObjectType::AIPawn, GetQuestProviderObject());
	if (OnObjectSpawnedDelegate == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::OnTaskCompleted: OnObjectSpawnedDelegate is nullptr!"));
		return;
	}

	OnObjectSpawnedDelegate->Remove(OnObjectSpawnedDelegateHandle);
	for (auto& [HealthComponent, DelegateHandle] : EnemyDestroyedDelegateHandleMap)
	{
		if (IsValid(HealthComponent))
		{
			HealthComponent->OnOwnerKilledDelegate.Remove(DelegateHandle);
		}
	}

	EnemyDestroyedDelegateHandleMap.Empty();
}

void UST_QuestTask_AIAreaClearing::StartWave(int32 WaveIndex)
{
	if (!EnemiesWavesAIAmount.IsValidIndex(WaveIndex))
	{
		FinishTask(EQuestTaskCompleteResult::Succeeded);
		return;
	}

	if (EnemiesWavesAIAmount[WaveIndex] <= 0)
	{
		StartWave(++CurrentWaveIndex);
		return;
	}

	OnAreaClearingWaveChangedDelegate.Broadcast(CurrentWaveIndex);
	SpawnEnemy();
}

void UST_QuestTask_AIAreaClearing::OnEnemySpawned(AActor* InTargetActor)
{
	UST_HealthComponent* HealthComponent = Cast<UST_HealthComponent>(InTargetActor->GetComponentByClass(UST_HealthComponent::StaticClass()));
	if (!HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_AreaClearingGameMode::OnEnemySpawned: Enemy has no HealthComponent!"));
		return;
	}

	auto OnEnemyKilledLambda = [this, HealthComponent]()
	{
		if (IsValid(HealthComponent))
		{
			EnemyDestroyedDelegateHandleMap.Remove(HealthComponent);
		}

		OnEnemyDestroyed();
	};

	EnemyDestroyedDelegateHandleMap.Add(HealthComponent, HealthComponent->OnOwnerKilledDelegate.AddLambda(OnEnemyKilledLambda));
	
	++CurrentWaveAIAmount;

	if (CurrentWaveAIAmount < EnemiesWavesAIAmount[CurrentWaveIndex])
	{
		SpawnEnemy();
	}
	else
	{
		OnAreaClearingWaveEnemyAmountChangedDelegate.Broadcast(CurrentWaveAIAmount);
	}
}

void UST_QuestTask_AIAreaClearing::OnEnemyDestroyed()
{
	OnAreaClearingWaveEnemyAmountChangedDelegate.Broadcast(--CurrentWaveAIAmount);
	if (CurrentWaveAIAmount <= 0)
	{
		StartWave(++CurrentWaveIndex);
	}
}

void UST_QuestTask_AIAreaClearing::SpawnEnemy()
{
	UObjectSpawnSubsystem* ObjectSpawnSubsystem = GetWorld()->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(ObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::OnTaskStarted: ObjectSpawnSubsystem is nullptr!"));
		return;
	}

	ObjectSpawnSubsystem->SpawnObject(ESpawnObjectType::AIPawn, GetQuestProviderObject());
}
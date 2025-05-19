#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/ObjectSpawnSubsystem/BaseObjectSpawner.h"
#include "TimerManager.h"

void UObjectSpawnManager::Initialize(const ESpawnObjectType InSpawnObjectType, const FObjectSpawnParameters& NewSpawnParameters, const UObject* SpawnerOwner)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnManager::Initialize: Failed to get world"));
		return;
	}

	SpawnObjectType = InSpawnObjectType;
	SpawnOwner = SpawnerOwner;

	TArray<AActor*> FoundSpawningActors;
	UGameplayStatics::GetAllActorsOfClass(World, ABaseObjectSpawner::StaticClass(), FoundSpawningActors);

	if (FoundSpawningActors.IsEmpty())
	{
		return;
	}

	for (AActor* FoundSpawingActor : FoundSpawningActors)
	{
		RegisterSpawner(Cast<ABaseObjectSpawner>(FoundSpawingActor));
	}

	SetSpawnTime(NewSpawnParameters.SpawnTime);
	SetDestroyTime(NewSpawnParameters.DestroyTime);
	SetMaxObjectsCount(NewSpawnParameters.MaxObjectsCount);

	SetIsSpawnLimited(NewSpawnParameters.bIsSpawnLimited);
	SetIsAutoSpawnEnabled(NewSpawnParameters.bIsAutoSpawnEnabled);
	SetIsAutoDestroyEnabled(NewSpawnParameters.bIsAutoDestroyEnabled);

	if (SpawnParams.bSpawnFromTheStart)
	{
		SpawnRandomObject();
	}
}

void UObjectSpawnManager::RegisterSpawner(ABaseObjectSpawner* SpawningActor)
{
	if (!SpawningActor || !SpawningActor->HasSpawnObjectType(SpawnObjectType))
	{
		return;
	}

	const bool bIsNeededOwner = SpawningActor->GetSpawnOwner() == SpawnOwner || (SpawningActor->GetSpawnOwner() == nullptr && SpawnOwner->IsA<UWorld>());
	if (!bIsNeededOwner)
	{
		return;
	}

	SpawningActor->OnSetSpawerEnabledDelegate.BindUObject(this, &ThisClass::OnSpawnerSetEnabled);
	SpawningActor->OnSpawnedObjectDestroyedDelegate.BindUObject(this, &ThisClass::OnSpawnedObjectDestroyed);
	SpawningActor->OnSpawnOwnerChangedDelegate.BindUObject(this, &ThisClass::OnSpawnerOwnerChanged);

	if (SpawningActor->CanSpawnObject())
	{
		AvailableSpawnActors.Add(SpawningActor);
	}
}

void UObjectSpawnManager::UnregisterSpawner(ABaseObjectSpawner* SpawningActor)
{
	if (!IsValid(SpawningActor) || !SpawningActor->OnSetSpawerEnabledDelegate.IsBoundToObject(this))
	{
		return;
	}

	SpawningActor->OnSetSpawerEnabledDelegate.Unbind();
	SpawningActor->OnSpawnedObjectDestroyedDelegate.Unbind();
	SpawningActor->OnSpawnOwnerChangedDelegate.Unbind();

	AvailableSpawnActors.Remove(SpawningActor);
}

void UObjectSpawnManager::BeginDestroy()
{
	Super::BeginDestroy();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);

		for (FTimerHandle& DestroyTimerHandle : DestroyTimerHandles)
		{
			World->GetTimerManager().ClearTimer(DestroyTimerHandle);
		}
	}
}

void UObjectSpawnManager::SetIsAutoSpawnEnabled(bool bIsEnabled)
{
	SpawnParams.bIsAutoSpawnEnabled = bIsEnabled;

	SetSpawnTimerEnabled(bIsEnabled);
}

void UObjectSpawnManager::SetIsSpawnLimited(bool bIsLimited)
{
	SpawnParams.bIsSpawnLimited = bIsLimited;
}

void UObjectSpawnManager::SetIsAutoDestroyEnabled(bool bIsEnabled)
{
	SpawnParams.bIsAutoDestroyEnabled = bIsEnabled;
}

void UObjectSpawnManager::SetSpawnTime(float NewSpawnTime)
{
	SpawnParams.SpawnTime = NewSpawnTime;

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnManager::SetSpawnTime: Failed to get world"));
		return;
	}

	// Restart timer with new duration
	if (SpawnParams.bIsAutoSpawnEnabled && World->GetTimerManager().IsTimerActive(SpawnTimerHandle))
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		SetSpawnTimerEnabled(true);
	}
}

void UObjectSpawnManager::SetDestroyTime(float NewDestroyTime)
{
	SpawnParams.DestroyTime = NewDestroyTime;
}

void UObjectSpawnManager::SetMaxObjectsCount(int32 Count)
{
	SpawnParams.MaxObjectsCount = Count;
}

AActor* UObjectSpawnManager::SpawnRandomObject()
{
	return SpawnRandomObject(FMath::RandRange(0, AvailableSpawnActors.Num() - 1));
}

AActor* UObjectSpawnManager::SpawnRandomObject(int32 SpawnVolumeIndex)
{
	if (SpawnVolumeIndex >= AvailableSpawnActors.Num())
	{
		return nullptr;
	}

	const bool bIsOverMaxCount = SpawnParams.bIsSpawnLimited && SpawnedObjectsCount >= SpawnParams.MaxObjectsCount;
	if (bIsOverMaxCount)
	{
		return nullptr;
	}

	auto SpawnActorsIt = AvailableSpawnActors.CreateIterator();
	for (int i = 0; i < SpawnVolumeIndex; ++i)
	{
		++SpawnActorsIt;
	}

	ABaseObjectSpawner* SpawningActor = *SpawnActorsIt;
	if (!IsValid(SpawningActor))
	{
		return nullptr;
	}

	AActor* NewObject = SpawningActor->SpawnRandomObject();
	OnObjectIsSpawned(SpawningActor, NewObject);

	return NewObject;
}

void UObjectSpawnManager::OnObjectIsSpawned(ABaseObjectSpawner* ObjectSpawner, AActor* SpawnedObject)
{
	if (!ObjectSpawner || !SpawnedObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::SpawnRandomObject: Spawn volume or object are not presented."));
		return;
	}

	if (SpawnParams.bIsAutoDestroyEnabled)
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnManager::OnObjectIsSpawned: Failed to get world"));
			return;
		}

		FSetElementId HandleID = DestroyTimerHandles.Add(FTimerHandle{});
		auto HandleDestroyTimerFinished = [this, SpawnedObject, HandleID]()
		{
			OnDestroyTimerFinished(SpawnedObject);
			DestroyTimerHandles.Remove(HandleID);
		};

		World->GetTimerManager().SetTimer(DestroyTimerHandles[HandleID], FTimerDelegate::CreateLambda(HandleDestroyTimerFinished), SpawnParams.DestroyTime, false);
	}

	SpawnedObjectsCount += 1;
	if (ObjectSpawner->HasMaxObjectsCount())
	{
		AvailableSpawnActors.Remove(ObjectSpawner);
	}

	const bool bIsOverMaxCount = SpawnParams.bIsSpawnLimited && SpawnedObjectsCount >= SpawnParams.MaxObjectsCount;
	if (bIsOverMaxCount || AvailableSpawnActors.IsEmpty())
	{
		SetSpawnTimerEnabled(false);
	}

	OnObjectSpawnedDelegate.Broadcast(ObjectSpawner, SpawnedObject);
}

void UObjectSpawnManager::OnDestroyTimerFinished(AActor* SpawnedObject)
{
	SpawnedObject->Destroy();
}

void UObjectSpawnManager::OnSpawnerSetEnabled(ABaseObjectSpawner* SpawningActor, bool bIsEnabled)
{
	if (!IsValid(SpawningActor))
	{
		return;
	}

	if (!bIsEnabled)
	{
		AvailableSpawnActors.Remove(SpawningActor);
	}
	else if (SpawningActor->CanSpawnObject())
	{
		AvailableSpawnActors.Add(SpawningActor);
	}
}

void UObjectSpawnManager::OnSpawnedObjectDestroyed(ABaseObjectSpawner* SpawningActor, AActor* SpawnedObject)
{
	if (!SpawningActor || !SpawnedObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::OnSpawnedObjectHandled: Spawn volume or object are not presented."));
		return;
	}

	SpawnedObjectsCount -= 1;
	if (SpawningActor->GetMaxObjectsCount() == SpawningActor->GetSpawnedObjectsCount() + 1)
	{
		AvailableSpawnActors.Add(SpawningActor);
	}

	if (SpawnParams.bIsAutoSpawnEnabled)
	{
		SetSpawnTimerEnabled(true);
	}
}

void UObjectSpawnManager::OnSpawnerOwnerChanged(ABaseObjectSpawner* SpawningActor, UObject* OldOwner, UObject* NewOwner)
{
	OnSpawnerOwnerChangedDelegate.ExecuteIfBound(SpawningActor, OldOwner, NewOwner);
}

void UObjectSpawnManager::SetSpawnTimerEnabled(bool bIsEnabled)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnManager::SetSpawnTimerEnabled: Failed to get world"));
		return;
	}

	bool bIsSpawnTimerActive = World->GetTimerManager().IsTimerActive(SpawnTimerHandle);

	if (bIsEnabled && !bIsSpawnTimerActive)
	{
		if (SpawnParams.SpawnTime == 0.f)
		{
			bool bIsSpawnNeeded = true;
			while (bIsSpawnNeeded)
			{
				bIsSpawnNeeded = (SpawnRandomObject() != nullptr);
			}
		}
		else
		{
			auto SpawnRandomObjectLambda = [this]() { SpawnRandomObject(); };
			World->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda(SpawnRandomObjectLambda), SpawnParams.SpawnTime, true);
		}
	}
	else if (!bIsEnabled && bIsSpawnTimerActive)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void UObjectSpawnManager::UpdateAvailableSpawners()
{

}

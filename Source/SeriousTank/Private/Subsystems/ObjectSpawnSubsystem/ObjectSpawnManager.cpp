#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnVolume.h"

void UObjectSpawnManager::Initialize(const ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& NewSpawnParameters)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnManager::Initialize: Failed to get world"));
		return;
	}

	TArray<AActor*> FoundVolumesActors;
	UGameplayStatics::GetAllActorsOfClass(World, AObjectSpawnVolume::StaticClass(), FoundVolumesActors);

	if (FoundVolumesActors.IsEmpty())
	{
		return;
	}

	AvailableSpawnVolumes.Reserve(FoundVolumesActors.Num());
	for (AActor* FoundVolumesActor : FoundVolumesActors)
	{
		AObjectSpawnVolume* SpawningVolume = Cast<AObjectSpawnVolume>(FoundVolumesActor);
		if (SpawningVolume && SpawningVolume->HasSpawnObjectType(SpawnObjectType))
		{
			AvailableSpawnVolumes.Add(SpawningVolume);

			SpawningVolume->OnSpawnedObjectHandled.BindUObject(this, &ThisClass::OnSpawnedObjectHandled);
		}
	}

	SetSpawnTime(NewSpawnParameters.SpawnTime);
	SetDestroyTime(NewSpawnParameters.DestroyTime);
	SetMaxObjectsCount(NewSpawnParameters.MaxObjectsCount);

	SetIsSpawnLimited(NewSpawnParameters.bIsSpawnLimited);
	SetIsAutoSpawnEnabled(NewSpawnParameters.bIsAutoSpawnEnabled);
	SetIsAutoDestroyEnabled(NewSpawnParameters.bIsAutoDestroyEnabled);
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

bool UObjectSpawnManager::SpawnRandomObject()
{
	return SpawnRandomObject(FMath::RandRange(0, AvailableSpawnVolumes.Num() - 1));
}

bool UObjectSpawnManager::SpawnRandomObject(int32 SpawnVolumeIndex)
{
	const bool bIsOverMaxCount = SpawnParams.bIsSpawnLimited && SpawnedObjectsCount >= SpawnParams.MaxObjectsCount;
	if (bIsOverMaxCount || AvailableSpawnVolumes.IsEmpty())
	{
		return false;
	}

	if (AvailableSpawnVolumes.IsValidIndex(SpawnVolumeIndex))
	{
		AObjectSpawnVolume* SpawningVolume = AvailableSpawnVolumes[SpawnVolumeIndex];
		AActor* NewObject = SpawningVolume->SpawnRandomObject();

		OnObjectIsSpawned(SpawningVolume, NewObject);
		return true;
	}

	return false;
}

void UObjectSpawnManager::OnObjectIsSpawned(AObjectSpawnVolume* SpawnVolume, AActor* SpawnedObject)
{
	if (!SpawnVolume || !SpawnedObject)
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
	if (SpawnVolume->HasMaxObjectsCount())
	{
		AvailableSpawnVolumes.Remove(SpawnVolume);
	}

	const bool bIsOverMaxCount = SpawnParams.bIsSpawnLimited && SpawnedObjectsCount >= SpawnParams.MaxObjectsCount;
	if (bIsOverMaxCount || AvailableSpawnVolumes.IsEmpty())
	{
		SetSpawnTimerEnabled(false);
	}
}

void UObjectSpawnManager::OnDestroyTimerFinished(AActor* SpawnedObject)
{
	SpawnedObject->Destroy();
}

void UObjectSpawnManager::OnSpawnedObjectHandled(AObjectSpawnVolume* SpawnVolume, AActor* SpawnedObject)
{
	if (!SpawnVolume || !SpawnedObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::OnSpawnedObjectHandled: Spawn volume or object are not presented."));
		return;
	}

	SpawnedObjectsCount -= 1;
	if (SpawnVolume->GetMaxObjectsCount() == SpawnVolume->GetSpawnedObjectsCount() + 1)
	{
		AvailableSpawnVolumes.Add(SpawnVolume);
	}

	if (SpawnParams.bIsAutoSpawnEnabled)
	{
		SetSpawnTimerEnabled(true);
	}
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
				bIsSpawnNeeded = SpawnRandomObject();
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

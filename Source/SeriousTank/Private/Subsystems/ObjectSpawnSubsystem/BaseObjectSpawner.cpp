#include "Subsystems/ObjectSpawnSubsystem/BaseObjectSpawner.h"

#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "Engine/World.h"

void ABaseObjectSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SpawnOwner))
	{
		SetOwner(SpawnOwner);
	}
}

void ABaseObjectSpawner::SetIsEnabled(bool bInIsEnabled)
{
	bIsEnabled = bInIsEnabled;
	OnSetSpawerEnabledDelegate.ExecuteIfBound(this, bIsEnabled);
}

bool ABaseObjectSpawner::CanSpawnObject() const
{
	return IsEnabled() && !HasMaxObjectsCount();
}

AActor* ABaseObjectSpawner::SpawnRandomObject()
{
	if (ClassesToSpawn.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseObjectSpawner::SpawnRandomObject: classes for spawning are not defined."));
		return nullptr;
	}

	return SpawnObject(ClassesToSpawn[FMath::RandRange(0, ClassesToSpawn.Num() - 1)]);
}

AActor* ABaseObjectSpawner::SpawnObject(TSubclassOf<AActor> ClassToSpawn)
{
	if (!bIsEnabled)
	{
		UE_LOG(LogTemp, Display, TEXT("ABaseObjectSpawner::SpawnObject: Spawner is disabled."));
		return nullptr;
	}

	if (MaxSpawnedObjects > 0 && SpawnedObjectsCount >= MaxSpawnedObjects)
	{
		UE_LOG(LogTemp, Display, TEXT("ABaseObjectSpawner::SpawnObject: max amount of object are already spawned."));
		return nullptr;
	}

	if (!ClassesToSpawn.Contains(ClassToSpawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseObjectSpawner::SpawnObject: class is not allowed for spawning."));
		return nullptr;
	};

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseObjectSpawner::SpawnObject: Failed to get world."));
		return nullptr;
	}

	AActor* SpawnedObject = World->SpawnActor<AActor>(ClassToSpawn, GetSpawnLocation(), FRotator{ 0.f });
	if (SpawnedObject)
	{
		SpawnedObject->OnDestroyed.AddDynamic(this, &ThisClass::OnSpanwedObjectDestroyed);

		SpawnedObjectsCount += 1;
	}

	return SpawnedObject;
}

void ABaseObjectSpawner::OnSpanwedObjectDestroyed(AActor* HandledObject)
{
	SpawnedObjectsCount -= 1;

	OnSpawnedObjectDestroyedDelegate.ExecuteIfBound(this, HandledObject);
}

FVector ABaseObjectSpawner::GetSpawnLocation() const
{
	return GetActorLocation();
}

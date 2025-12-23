#include "Subsystems/ObjectSpawnSubsystem/BaseObjectSpawner.h"

#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"
#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "Engine/World.h"

ABaseObjectSpawner::ABaseObjectSpawner()
{
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneRootComponent);
}

void ABaseObjectSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetSpawnOwner(SpawnOwner);
}

void ABaseObjectSpawner::SetSpawnOwner(UObject* InSpawnOwner)
{
	if (SpawnOwner == InSpawnOwner)
	{
		return;
	}

	UObject* OldSpawnOwner = SpawnOwner;
	SpawnOwner = InSpawnOwner;

	if (OldSpawnOwner == nullptr)
	{
		UObjectSpawnSubsystem* SpawnSubsystem = GetWorld()->GetSubsystem<UObjectSpawnSubsystem>();
		if (IsValid(SpawnSubsystem))
		{
			SpawnSubsystem->RegisterSpawner(this);
		}
	}
	else if (SpawnOwner != InSpawnOwner)
	{
		OnSpawnOwnerChangedDelegate.ExecuteIfBound(this, OldSpawnOwner, SpawnOwner);
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
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid spawn owner or already set."), *GetClass()->GetName(), TEXT("SpawnRandomObject"));
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

		OnSpawnerSpawnedObjectDelegete.Broadcast(SpawnedObject);
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

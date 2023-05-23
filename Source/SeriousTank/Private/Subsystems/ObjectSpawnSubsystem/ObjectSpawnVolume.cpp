#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"

AObjectSpawnVolume::AObjectSpawnVolume()
{
	SpawningBoxComponent = CreateDefaultSubobject<UBoxComponent>("SpawningBoxComponent");
	SetRootComponent(SpawningBoxComponent);

	SetActorEnableCollision(false);
	bNetLoadOnClient = false;
}

AActor* AObjectSpawnVolume::SpawnRandomObject()
{
	if (ClassesToSpawn.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("AObjectSpawnVolume::SpawnRandomObject: classes for spawning are not defined."));
		return nullptr;
	}

	if (SpawnedObjectsCount >= MaxSpawnedObjects)
	{
		UE_LOG(LogTemp, Display, TEXT("AObjectSpawnVolume::SpawnRandomObject: max amount of object are already spawned."));
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("AObjectSpawnVolume::SpawnRandomObject: Failed to get world."));
		return nullptr;
	}

	TSubclassOf<AActor> ActorClass = ClassesToSpawn[FMath::RandRange(0, ClassesToSpawn.Num() - 1)];
	AActor* SpawnedObject = World->SpawnActor<AActor>(ActorClass, GetRandomSpawnLocation(), FRotator{ 0.f });

	if (SpawnedObject)
	{
		RegisterObjectHandlers(SpawnedObject);
		SpawnedObjectsCount += 1;
	}

	return SpawnedObject;
}

void AObjectSpawnVolume::RegisterObjectHandlers(AActor* NewObject)
{
	if (!NewObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnVolume::RegisterObjectHandlers: Failed to get valid object."));
		return;
	}

	NewObject->OnDestroyed.AddDynamic(this, &ThisClass::OnObjectHandled);
}

void AObjectSpawnVolume::OnObjectHandled(AActor* HandledObject)
{
	SpawnedObjectsCount -= 1;

	OnSpawnedObjectHandled.ExecuteIfBound(this, HandledObject);
}

FVector AObjectSpawnVolume::GetRandomSpawnLocation() const
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	const FBox SpawnBox{ Origin - BoxExtent, Origin + BoxExtent };

	return FVector(FMath::FRandRange(SpawnBox.Min.X, SpawnBox.Max.X),
		FMath::FRandRange(SpawnBox.Min.Y, SpawnBox.Max.Y),
		SpawnBox.Min.Z + (SpawnBox.Max.Z - SpawnBox.Min.Z) / 2);
}

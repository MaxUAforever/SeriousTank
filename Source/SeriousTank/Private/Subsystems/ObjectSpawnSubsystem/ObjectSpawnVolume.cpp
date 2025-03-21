#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"

AObjectSpawnVolume::AObjectSpawnVolume()
{
	SpawningBoxComponent = CreateDefaultSubobject<UBoxComponent>("SpawningBoxComponent");
	SetRootComponent(SpawningBoxComponent);

	SetActorEnableCollision(false);
}

FVector AObjectSpawnVolume::GetSpawnLocation() const
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	const FBox SpawnBox{ Origin - BoxExtent, Origin + BoxExtent };

	return FVector(FMath::FRandRange(SpawnBox.Min.X, SpawnBox.Max.X),
		FMath::FRandRange(SpawnBox.Min.Y, SpawnBox.Max.Y),
		SpawnBox.Min.Z + (SpawnBox.Max.Z - SpawnBox.Min.Z) / 2);
}

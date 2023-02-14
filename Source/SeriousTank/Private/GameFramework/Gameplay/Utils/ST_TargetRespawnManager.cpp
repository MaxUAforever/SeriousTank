#include "GameFramework/Gameplay/Utils/ST_TargetRespawnManager.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "Actors/GameplayActors/ST_TargetSpawningVolume.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(TargetRespawnManager, Display, All);

void UST_TargetRespawnManager::SetSpawnVolumeClass(TSubclassOf<AST_TargetSpawningVolume> NewSpawnVolumeClass)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	SpawnVolumeClass = NewSpawnVolumeClass;

	TArray<AActor*> FoundVolumesActors;
	UGameplayStatics::GetAllActorsOfClass(World, SpawnVolumeClass, FoundVolumesActors);

	SpawnVolumes.Reserve(FoundVolumesActors.Num());
	for (AActor* FoundVolumesActor : FoundVolumesActors)
	{
		if (AST_TargetSpawningVolume* SpawningVolume = Cast<AST_TargetSpawningVolume>(FoundVolumesActor))
		{
			SpawnVolumes.Add(SpawningVolume);
		}
	}
}

AST_ShootTarget* UST_TargetRespawnManager::SpawnTarget()
{
	if (!ShootTargetClass)
	{
		UE_LOG(TargetRespawnManager, Warning, TEXT("Target class for spawning is not defined."));
		return nullptr;
	}

	if (!SpawnVolumeClass)
	{
		UE_LOG(TargetRespawnManager, Warning, TEXT("Class for spawn volumes is not defined."));
		return nullptr;
	}

	if (UWorld* World = GetWorld())
	{
		if (AST_TargetSpawningVolume* SpawnVolume = GetRandomSpawnVolume())
		{
			FVector TargetLocation = GetRandomSpawnLocation(SpawnVolume);

			return World->SpawnActor<AST_ShootTarget>(ShootTargetClass, TargetLocation, FRotator{0.f});
		}
	}

	return nullptr;
}

AST_TargetSpawningVolume* UST_TargetRespawnManager::GetRandomSpawnVolume()
{
	if (SpawnVolumes.Num() == 0)
	{
		UE_LOG(TargetRespawnManager, Warning, TEXT("No spawning volumes are located in the world."));
		return nullptr;
	}

	return SpawnVolumes[FMath::RandRange(0, SpawnVolumes.Num() - 1)];
}

FVector UST_TargetRespawnManager::GetRandomSpawnLocation(const AST_TargetSpawningVolume* SpawnVolume)
{
	FVector Origin;
	FVector BoxExtent;
	SpawnVolume->GetActorBounds(false, Origin, BoxExtent);

	const FBox SpawnBox{Origin - BoxExtent, Origin + BoxExtent};
	
	return FVector(FMath::FRandRange(SpawnBox.Min.X, SpawnBox.Max.X),
				   FMath::FRandRange(SpawnBox.Min.Y, SpawnBox.Max.Y),
				   SpawningHeight);
}

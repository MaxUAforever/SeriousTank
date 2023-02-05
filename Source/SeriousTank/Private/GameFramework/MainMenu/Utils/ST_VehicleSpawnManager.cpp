#include "GameFramework/MainMenu/Utils/ST_VehicleSpawnManager.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Core/ST_CoreTypes.h"

#include "Components/AudioComponent.h"
#include "Engine/TargetPoint.h"
#include "Engine/World.h"
#include "EngineUtils.h"

void UST_VehicleSpawnManager::SpawnVehicle(FVehicleInfo VehicleInfo)
{
	UWorld* World = IsValid(Vehicle) ? Vehicle->GetWorld() : GetWorld();
	if (!World)
	{
		return;
	}
    
	if (Vehicle)
	{
		Vehicle->Destroy();
	}

	TActorIterator<AActor> It(World, ATargetPoint::StaticClass());
	if (It)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		Vehicle = World->SpawnActor<AST_BaseVehicle>(VehicleInfo.VehicleClass, It->GetActorLocation(), FRotator{ 0.f }, SpawnParameters);
		DisableSoundsOnVehicle();
	}
}

void UST_VehicleSpawnManager::DisableSoundsOnVehicle()
{
	if (!Vehicle)
	{
		return;
	}

	TArray<UActorComponent*> AuidoComponents;
	Vehicle->GetComponents(UAudioComponent::StaticClass(), AuidoComponents);

	for (UActorComponent* ActorComponent : AuidoComponents)
	{
		if (UAudioComponent* AudioComponent = Cast<UAudioComponent>(ActorComponent))
		{
			AudioComponent->SetVolumeMultiplier(0.f);
		}
	}
}

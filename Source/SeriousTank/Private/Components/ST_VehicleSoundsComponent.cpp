#include "Components/ST_VehicleSoundsComponent.h"

#include "Core/ST_CoreTypes.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY(VehicleSoundsComponent);

void UST_VehicleSoundsComponent::PlayMovingSound(EMovingType MovingType)
{
	USoundCue* NewSound = GetMovingSound(MovingType);
	if (NewSound && CurrentSound != NewSound)
	{
		SetSound(NewSound);
		CurrentSound = NewSound;

		Play();
	}
}

USoundCue* UST_VehicleSoundsComponent::GetMovingSound(EMovingType MovingType) const
{
	switch (MovingType)
	{
		case EMovingType::Braking: 
			return BrakingVehicleSound;
		case EMovingType::ForceMoving:
			return ForceMovingVehicleSound;
		case EMovingType::InertiaMoving:
			return InertiaMovingVehicleSound;
		case EMovingType::RotatingInPlace:
			return RotatingVehicleSound;
		case EMovingType::Standing:
			return StandingVehicleSound;
		default:
			UE_LOG(VehicleSoundsComponent, Warning, TEXT("GetSound: Unexpected EMovingType value."));
			return nullptr;
	}
}

#pragma once

#include "Components/AudioComponent.h"
#include "ST_VehicleSoundsComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(VehicleSoundsComponent, Warning, All);

class USoundCue;
enum class EMovingType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_VehicleSoundsComponent : public UAudioComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* StandingVehicleSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* ForceMovingVehicleSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* InertiaMovingVehicleSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* RotatingVehicleSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* BrakingVehicleSound;

private:
	USoundCue* CurrentSound;

public:
	void PlayMovingSound(EMovingType MovingType);

private:
	USoundCue* GetSound(EMovingType MovingType) const;
};

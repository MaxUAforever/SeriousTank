#pragma once

#include "Actors/BaseTrackedVehicle.h"
#include "TrackedTank.generated.h"

/**
 * 
 */
UCLASS()
class SERIOUSTANK_API ATrackedTank : public ABaseTrackedVehicle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* TurretSceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BarrelMeshComponent;

protected:
	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed;

public:
	ATrackedTank();

protected:
	virtual void Tick(float DeltaTime) override;

private:
	void RotateTurretToCamera(float DeltaTime);
};

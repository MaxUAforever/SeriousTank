#pragma once

#include "Actors/BaseVehicle.h"
#include "BaseTrackedVehicle.generated.h"

class UBoxComponent;
class UCameraComponent;
class UST_TrackMovementComponent;

UCLASS()
class SERIOUSTANK_API ABaseTrackedVehicle : public ABaseVehicle
{
	GENERATED_BODY()

public:
	ABaseTrackedVehicle();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* VehicleSceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseStaticMeshComponent;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BaseCollisionComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* CameraSceneComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	UST_TrackMovementComponent* TrackMovementComponent;

private:
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void RotateCamera(float Value) override;
};

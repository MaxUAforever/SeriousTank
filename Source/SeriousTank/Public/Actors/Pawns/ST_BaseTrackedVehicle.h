#pragma once

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Core/ST_CoreTypes.h"

#include "ST_BaseTrackedVehicle.generated.h"

class UBoxComponent;
class UCameraComponent;
class UST_TrackMovementComponent;
class UST_VehicleSoundsComponent;
class UST_ViewAreaBoxComponent;

UCLASS()
class SERIOUSTANK_API AST_BaseTrackedVehicle : public AST_BaseVehicle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BaseCollisionComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseStaticMeshComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* CameraSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UST_ViewAreaBoxComponent* CameraViewAreaComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	UST_VehicleSoundsComponent* VehicleSoundComponent;

	UPROPERTY(EditAnywhere)
	UST_TrackMovementComponent* TrackMovementComponent;

public:
	AST_BaseTrackedVehicle();

	virtual UPawnMovementComponent* GetMovementComponent() const override;

protected:
	virtual void BeginPlay() override;

private:
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void RotateCamera(float Value) override;

	UFUNCTION()
	void OnMovingTypeChanged(EMovingType NewMovingType);

	void UpdateCameraViewArea();
};

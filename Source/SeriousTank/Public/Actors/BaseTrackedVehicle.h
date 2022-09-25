#pragma once

#include "GameFramework/Pawn.h"
#include "BaseTrackedVehicle.generated.h"

class UBoxComponent;
class UCameraComponent;

enum class EDirection : uint8
{
	NONE,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

UCLASS()
class SERIOUSTANK_API ABaseTrackedVehicle : public APawn
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

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxRotationRadius;

	UPROPERTY(EditAnywhere)
	int32 MaxSpeed;

	UPROPERTY(EditAnywhere)
	int32 MaxBackwardSpeed;

	UPROPERTY(EditAnywhere)
	float AcselerationValue;

	UPROPERTY(EditAnywhere)
	float InertiaValue;

	UPROPERTY(EditAnywhere)
	float BreakAcselerationValue;

private:
	// Filled with non-zero values if user initiate moving.
	// X-Axis is used for forward(+1) / backward(-1) moving,
	// Y-Axis is used for right(+1) / left(-1) rotation. 
	FVector RequestedDirections;
	
	float CurrentSpeed;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void CalculatePosition(float DeltaTime);

	virtual float CalculateAcceleration();
	
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateCamera(float Value);

	void CalculateCurrentSpeed(float DeltaTime);
};

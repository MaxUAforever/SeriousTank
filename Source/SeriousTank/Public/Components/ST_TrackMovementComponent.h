#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ST_TrackMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_TrackMovementComponent : public UActorComponent
{
	GENERATED_BODY()

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

	UPROPERTY(VisibleAnywhere)
	float CurrentSpeed;

private:
	// Filled with non-zero values if user initiate moving.
	// X-Axis is used for forward(+1) / backward(-1) moving,
	// Y-Axis is used for right(+1) / left(-1) rotation. 
	FVector RequestedDirections;

public:	
	UST_TrackMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

protected:
	void CalculatePosition(float DeltaTime);

	void CalculateCurrentSpeed(float DeltaTime);
	
	float CalculateAcceleration();
};

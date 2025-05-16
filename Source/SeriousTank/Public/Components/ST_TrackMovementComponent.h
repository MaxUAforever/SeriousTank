#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "ST_TrackMovementComponent.generated.h"

enum class EMovingType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_TrackMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnMovingTypeChanged, EMovingType)
	FOnMovingTypeChanged OnMovingTypeChanged;

public:	
	UST_TrackMovementComponent();

	float GetCurrentSpeed() const { return CurrentSpeed; }
	virtual float GetMaxSpeed() const override;

	EMovingType GetMovingType() const;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual void RequestPathMove(const FVector& MoveInput) override;

protected:
	void CalculatePosition(float DeltaTime);

	void CalculateCurrentSpeed(float DeltaTime);
	
	float CalculateAcceleration();
    
    virtual FVector ConstrainLocationToPlane(FVector Location) const override;

	float GetCurrentBreakingDistance() const;

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

	UPROPERTY(EditAnywhere, Category = "Navigation")
	float BreakingDistanceCoef;

	UPROPERTY(EditAnywhere, Category = "Navigation")
	float MinAngleToAccelerate;

	UPROPERTY(EditAnywhere, Category = "Navigation")
	float MinPathFollowSpeed;

	UPROPERTY(EditAnywhere, Category = "Navigation|Debug")
	bool bDrawDebugNavigationPath;

	UPROPERTY(EditAnywhere, Category = "Navigation|Debug")
	float DrawHeightOffset;

private:
	// Filled with non-zero values if user initiate moving.
	// X-Axis is used for forward(+1) / backward(-1) moving,
	// Y-Axis is used for right(+1) / left(-1) rotation. 
	FVector RequestedDirections;

	float CurrentSpeed;
	EMovingType CurrentMovingType;
};

#include "Components/ST_TrackMovementComponent.h"

#include "Core/ST_CoreTypes.h"
#include "GameFramework/Actor.h"

UST_TrackMovementComponent::UST_TrackMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxRotationRadius = 350;
	MaxSpeed = 140;
	MaxBackwardSpeed = 40;
	AcselerationValue = 30;
	InertiaValue = 30;
	BreakAcselerationValue = 100;

	bConstrainToPlane = true;
	bSnapToPlaneAtStart = true;
	SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);

	CurrentSpeed = 0;
}

EMovingType UST_TrackMovementComponent::GetMovingType() const
{
	const bool IsForceMoving = RequestedDirections.X != 0;
	if (IsForceMoving)
	{
		bool bIsBraking = CurrentSpeed < 0 != RequestedDirections.X < 0;
		return bIsBraking ? EMovingType::Braking : EMovingType::ForceMoving;
	}

	if (CurrentSpeed == 0)
	{
		const bool IsRotating = RequestedDirections.Y != 0;
		return IsRotating ? EMovingType::RotatingInPlace : EMovingType::Standing;
	}

	return EMovingType::InertiaMoving;
}

void UST_TrackMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculatePosition(DeltaTime);

	const EMovingType NewMovingType = GetMovingType();
	if (CurrentMovingType != NewMovingType)
	{
		CurrentMovingType = NewMovingType;
		OnMovingTypeChanged.ExecuteIfBound(NewMovingType);
	}
}

void UST_TrackMovementComponent::MoveForward(const float Value)
{
	RequestedDirections.X = Value;
}

void UST_TrackMovementComponent::MoveRight(const float Value)
{
	RequestedDirections.Y = Value;
}

void UST_TrackMovementComponent::CalculatePosition(float DeltaTime)
{
	if (RequestedDirections.IsZero() && CurrentSpeed == 0)
	{
		return;
	}

	CalculateCurrentSpeed(DeltaTime);

	// Direction of moving (forward or backward)
	const FRotator VehicleRotation = GetOwner()->GetActorRotation();
	const FVector CurrentDirectionVector = FMath::Sign(CurrentSpeed) * VehicleRotation.Vector();

	const float MovementDistance = FMath::Abs(CurrentSpeed) * DeltaTime;
	const bool IsTurningSideways = RequestedDirections.Y != 0;

	// Calculate location for moving without rotation.
	if (CurrentSpeed != 0 && !IsTurningSideways)
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(CurrentDirectionVector * MovementDistance, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit())
		{
			CurrentSpeed = 0.f;
		}

		return;
	}

	// Used for calculation of rotaion value with moving or without it.
	const float RotationMovementDistance = CurrentSpeed != 0 ? MovementDistance : MaxSpeed * DeltaTime;
	const float RotationRadius = CurrentSpeed != 0 ? (FMath::Abs(CurrentSpeed) / MaxSpeed) * MaxRotationRadius : MaxRotationRadius;

	// Angle from turning center between current position and new possible possition.
	const float TurningAngle = (RotationMovementDistance * 180) / (RotationRadius * PI);
	const FRotator UpdatedRotation = UpdatedComponent->GetComponentRotation() + FRotator{ 0, TurningAngle * RequestedDirections.Y, 0 };

	// Calculate location for moving with rotation.
	FVector MovementVector{0.f};
	if (CurrentSpeed != 0)
	{
		const FVector VectorToCurrentLocation = CurrentDirectionVector.RotateAngleAxis(-90.f * RequestedDirections.Y, FVector::ZAxisVector) * RotationRadius;
		const FVector VectorToNewLocation = VectorToCurrentLocation.RotateAngleAxis(TurningAngle * RequestedDirections.Y * -1, FVector::ZAxisVector);

		MovementVector = VectorToCurrentLocation - VectorToNewLocation;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(MovementVector, UpdatedRotation, true, Hit);
	if (Hit.IsValidBlockingHit())
	{
		CurrentSpeed = 0.f;
	}
}

float UST_TrackMovementComponent::CalculateAcceleration()
{
	const bool IsTryingToStop = (CurrentSpeed > 0 && RequestedDirections.X < 0) || (CurrentSpeed < 0 && RequestedDirections.X > 0);

	// Acceleration in case of force moving in opposite side of current velocity direction.
	if (IsTryingToStop)
	{
		return BreakAcselerationValue * FMath::Sign(CurrentSpeed) * -1;
	}

	const bool IsForcingMove = RequestedDirections.X != 0;
	const bool IsMovingBackward = RequestedDirections.X < 0 || CurrentSpeed < 0;

	// Acceleration value is calculated with quadratic function in case of force moving:
	// y = -a*x^2+b; a = b/X^2, where:
	// y - acceleration value, x - current speed, b - max acceleration value and X - max speed.
	// So aceleration is dectremented from max value to zero during range of speed values.
	if (IsForcingMove)
	{
		static const float ForwardAccelerationCoef = AcselerationValue / (MaxSpeed * MaxSpeed);
		static const float BackwardaccelerationCoef = AcselerationValue / (MaxBackwardSpeed * MaxBackwardSpeed);
		const float AccelerationCoef = IsMovingBackward ? BackwardaccelerationCoef : ForwardAccelerationCoef;

		return (AcselerationValue - (AccelerationCoef * CurrentSpeed * CurrentSpeed)) * (IsMovingBackward ? -1 : 1);
	}

	// Acceleration in case of only inertia moving.
	return InertiaValue * (IsMovingBackward ? 1 : -1);
}

void UST_TrackMovementComponent::CalculateCurrentSpeed(const float DeltaTime)
{
	const float Acseleration = CalculateAcceleration();

	const bool IsMovingForward = RequestedDirections.X > 0 || CurrentSpeed > 0;
	const bool IsMovingBackward = RequestedDirections.X < 0 || CurrentSpeed < 0;

	CurrentSpeed = FMath::Clamp(CurrentSpeed + Acseleration * DeltaTime,
		IsMovingBackward ? static_cast<float>(MaxBackwardSpeed) * -1 : 0,
		IsMovingForward ? static_cast<float>(MaxSpeed) : 0);

	//UE_LOG(BaseTrackLog, Warning, TEXT("Speed: %f, Acseleration: %f"), CurrentSpeed, Acseleration);
}


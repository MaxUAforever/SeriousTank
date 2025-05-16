#include "Components/ST_TrackMovementComponent.h"

#include "AIController.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Navigation/PathFollowingComponent.h"

namespace
{

void DrawDebugDirections(const UWorld* World, const FNavPathSharedPtr Path, const FVector& PawnLocation, const FVector& MoveInput, const FVector& RequestedDirection, const FVector& ForwardVector, const FVector& RightVector, float HeightOffset)
{
	TOptional<FVector> PreviousPathPoint;
	for (int Index = 0; Index < Path->GetPathPoints().Num(); Index++)
	{
		FVector DebugPathPointLocation = *Path->GetPathPointLocation(Index);
		DebugPathPointLocation.Z = HeightOffset;

		DrawDebugSphere(World, DebugPathPointLocation, 25.f, 12, FColor::Blue);
		if (PreviousPathPoint.IsSet())
		{
			DrawDebugLine(World, PreviousPathPoint.GetValue(), DebugPathPointLocation, FColor::Blue);
		}

		PreviousPathPoint = DebugPathPointLocation;
	}

	const FVector DebugPawnPointLocation = { PawnLocation.X , PawnLocation.Y, HeightOffset };
	const FVector MoveInputPointLocation = { PawnLocation.X , PawnLocation.Y, HeightOffset + 10.f };
	const FVector RequestedDirectionPointLocation = { PawnLocation.X , PawnLocation.Y, HeightOffset + 20.f };

	DrawDebugSphere(World, DebugPawnPointLocation, 25.f, 12, FColor::Red, false);
	DrawDebugLine(World, DebugPawnPointLocation, DebugPawnPointLocation + ForwardVector * 70.f, FColor::Red, false, -1.0f, 0U, 3.f);
	DrawDebugLine(World, MoveInputPointLocation, MoveInputPointLocation + MoveInput * 70.f, FColor::Green, false, -1.0f, 0U, 3.f);
	DrawDebugLine(World, RequestedDirectionPointLocation, RequestedDirectionPointLocation + (ForwardVector * RequestedDirection.X + RightVector * RequestedDirection.Y) * 70.f, FColor::Yellow, false, -1.0f, 0U, 3.f);
}

} // anonymous namespace

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

	bUseAccelerationForPaths = true;

	BreakingDistanceCoef = 1.5f;
	MinAngleToAccelerate = 15.f;
	MinPathFollowSpeed = 10.f;
	bDrawDebugNavigationPath = false;
	DrawHeightOffset = 250.f;

	CurrentSpeed = 0;
}

void UST_TrackMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* CurrentPawnOwner = Cast<APawn>(GetOwner()))
	{
		CurrentPawnOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnControllerChanged);
	}
}

void UST_TrackMovementComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (APawn* CurrentPawnOwner = Cast<APawn>(GetOwner()))
	{
		CurrentPawnOwner->ReceiveControllerChangedDelegate.RemoveDynamic(this, &ThisClass::OnControllerChanged);
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PathStopMovementsTimer);
	}
}

float UST_TrackMovementComponent::GetMaxSpeed() const
{
	return MaxSpeed;
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

void UST_TrackMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	const AController* OwnerController = GetController();
	const UPathFollowingComponent* PathFollowingComponent = IsValid(OwnerController) ? OwnerController->GetComponentByClass<UPathFollowingComponent>() : nullptr;
	if (!IsValid(PathFollowingComponent))
	{
		return;
	}

	const FNavPathSharedPtr Path = PathFollowingComponent->GetPath();
	if (!Path.IsValid() || !Path->IsValid())
	{
		return;
	}

	FVector FlatMoveInput = { MoveInput.X, MoveInput.Y, 0.f };
	FlatMoveInput.Normalize();

	const FVector ForwardVector = UpdatedComponent->GetForwardVector();
	const float AngleRadians = FMath::Acos(FVector::DotProduct(ForwardVector, FlatMoveInput));
	const float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	const FVector Cross = FVector::CrossProduct(ForwardVector, FlatMoveInput);
	const float RotationDirection = FMath::Sign(Cross.Z);

	RequestedDirections.Y = AngleDegrees > 2.f ? RotationDirection : 0.f;
	RequestedDirections.X = 0.f;

	if (AngleDegrees > 90.f && CurrentSpeed > MinPathFollowSpeed)
	{
		RequestedDirections.X = -1.f;
	}
	else if (AngleDegrees < MinAngleToAccelerate)
	{
		const FVector PathEnd = Path->GetEndLocation();
		const FVector::FReal DistToEndSq = FVector::DistSquared(GetActorFeetLocation(), PathEnd);
		const bool bShouldDecelerate = DistToEndSq < FMath::Square(GetCurrentBreakingDistance() * BreakingDistanceCoef);

		RequestedDirections.X = bShouldDecelerate && CurrentSpeed > MinPathFollowSpeed ? -1.f : 1.f;
	}

	if (bDrawDebugNavigationPath)
	{
		DrawDebugDirections(GetWorld(), Path, GetActorFeetLocation(), FlatMoveInput, RequestedDirections, ForwardVector, UpdatedComponent->GetRightVector(), DrawHeightOffset);
	}
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

FVector UST_TrackMovementComponent::ConstrainLocationToPlane(FVector Location) const
{
    FVector OffsetVector{0.f};
    if (AActor* CompOwner = GetOwner())
    {
        FVector Origin;
        FVector BoxExtent;
        float SphereRadius;
        UKismetSystemLibrary::GetComponentBounds(CompOwner->GetRootComponent(), Origin, BoxExtent, SphereRadius);
        
        OffsetVector.Z = BoxExtent.Z;
    }
    
    return Super::ConstrainLocationToPlane(Location) + OffsetVector;
}

float UST_TrackMovementComponent::GetCurrentBreakingDistance() const
{
	return FMath::Square(CurrentSpeed) / (2 * BreakAcselerationValue);
}

void UST_TrackMovementComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (IsValid(CurrentPathFollowingComponent))
	{
		CurrentPathFollowingComponent->OnRequestFinished.RemoveAll(this);
	}

	UPathFollowingComponent* PathFollowingComponent = IsValid(NewController) ? NewController->GetComponentByClass<UPathFollowingComponent>() : nullptr;
	if (IsValid(PathFollowingComponent))
	{
		CurrentPathFollowingComponent = PathFollowingComponent;
		CurrentPathFollowingComponent->OnRequestFinished.AddUObject(this, &ThisClass::OnPathFollowingFinished);
	}
}

void UST_TrackMovementComponent::OnPathFollowingFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(PathStopMovementsTimer, this, &ThisClass::StopFollowingMovements, 0.2f, true);
	}
}

void UST_TrackMovementComponent::StopFollowingMovements()
{
	if (IsValid(CurrentPathFollowingComponent) && CurrentPathFollowingComponent->GetStatus() == EPathFollowingStatus::Moving)
	{
		GetWorld()->GetTimerManager().ClearTimer(PathStopMovementsTimer);
		return;
	}

	static const float AcceptanceStopSpeed = 30.f;
	const bool bStoppedMovement = FMath::Abs(CurrentSpeed) < AcceptanceStopSpeed;

	RequestedDirections.Y = 0.f;
	if (bStoppedMovement)
	{
		RequestedDirections.X = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(PathStopMovementsTimer);
	}
	else
	{
		RequestedDirections.X = FMath::Sign(CurrentSpeed) * -1;
	}
}

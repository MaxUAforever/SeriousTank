#include "Actors/BaseTrackedVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"

DEFINE_LOG_CATEGORY_STATIC(BaseTrackLog, Display, All);

ABaseTrackedVehicle::ABaseTrackedVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentSpeed = 0;

	MaxRotationRadius = 350;
	MaxSpeed = 140;
	MaxBackwardSpeed = 40;
	AcselerationValue = 30;
	InertiaValue = 30;
	BreakAcselerationValue = 100;

 	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	VehicleSceneComponent = CreateDefaultSubobject<USceneComponent>("VehicleSceneComponent");
	VehicleSceneComponent->SetupAttachment(RootComponent);

	BaseStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	BaseStaticMeshComponent->SetupAttachment(VehicleSceneComponent);

	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	BaseCollisionComponent->SetupAttachment(VehicleSceneComponent);

	CameraSceneComponent = CreateDefaultSubobject<USceneComponent>("CameraSceneComponent");
	CameraSceneComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraSceneComponent);
}

void ABaseTrackedVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseTrackedVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculatePosition(DeltaTime);
}

void ABaseTrackedVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("RotateCamera"), this, &ThisClass::RotateCamera);
}

void ABaseTrackedVehicle::MoveForward(const float Value)
{
	if (Controller != nullptr)
	{
		RequestedDirections.X = Value;
	}
}

void ABaseTrackedVehicle::MoveRight(const float Value)
{
	if (Controller != nullptr)
	{
		RequestedDirections.Y = Value;
	}
}

void ABaseTrackedVehicle::RotateCamera(float Value)
{
	if (Controller != nullptr)
	{
		CameraSceneComponent->AddLocalRotation(FRotator{ 0, Value, 0 });
	}
}

void ABaseTrackedVehicle::CalculatePosition(float DeltaTime)
{
	if (RequestedDirections.IsZero() && CurrentSpeed == 0)
	{
		return;
	}

	CalculateCurrentSpeed(DeltaTime);

	// Direction of moving (forward or backward)
	const FRotator VehicleRotation = VehicleSceneComponent->GetComponentRotation();
	const FVector CurrentDirectionVector = FMath::Sign(CurrentSpeed) * VehicleRotation.Vector();
	
	const float MovementDistance = FMath::Abs(CurrentSpeed) * DeltaTime;
	const bool IsTurningSideways = RequestedDirections.Y != 0;

	// Calculate location for moving without rotation.
	if (CurrentSpeed != 0 && !IsTurningSideways)
	{
		SetActorLocation(GetActorLocation() + CurrentDirectionVector * MovementDistance);
		return;
	}
	
	// Used for calculation of rotaion value with moving or without it.
	const float RotationMovementDistance = CurrentSpeed != 0 ? MovementDistance : MaxSpeed * DeltaTime;
	const float RotationRadius = CurrentSpeed != 0 ? (FMath::Abs(CurrentSpeed) / MaxSpeed) * MaxRotationRadius : MaxRotationRadius;

	// Angle from turning center between current position and new possible possition.
	const float TurningAngle = (RotationMovementDistance * 180) / (RotationRadius * PI);
	VehicleSceneComponent->AddLocalRotation(FRotator{ 0, TurningAngle * RequestedDirections.Y , 0 });
	
	// Calculate location for moving with rotation.
	if (CurrentSpeed != 0)
	{
		const FVector VectorToCurrentLocation = CurrentDirectionVector.RotateAngleAxis(-90.f * RequestedDirections.Y, FVector::ZAxisVector) * RotationRadius;
		const FVector VectorToNewLocation = VectorToCurrentLocation.RotateAngleAxis(TurningAngle * RequestedDirections.Y * -1, FVector::ZAxisVector);

		const FVector NovementVector = VectorToCurrentLocation - VectorToNewLocation;
		
		SetActorLocation(GetActorLocation() + NovementVector);
	}
}

float ABaseTrackedVehicle::CalculateAcceleration()
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

void ABaseTrackedVehicle::CalculateCurrentSpeed(const float DeltaTime)
{
	const float Acseleration = CalculateAcceleration();

	const bool IsMovingForward = RequestedDirections.X > 0 || CurrentSpeed > 0;
	const bool IsMovingBackward = RequestedDirections.X < 0 || CurrentSpeed < 0;
	
	CurrentSpeed = FMath::Clamp(CurrentSpeed + Acseleration * DeltaTime,
								IsMovingBackward ? static_cast<float>(MaxBackwardSpeed) * -1 : 0,
								IsMovingForward ? static_cast<float>(MaxSpeed) : 0);

	UE_LOG(BaseTrackLog, Warning, TEXT("Speed: %f, Acseleration: %f"), CurrentSpeed, Acseleration);
}



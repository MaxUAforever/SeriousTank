#include "Actors/BaseTrackedVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_TrackMovementComponent.h"

#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"

DEFINE_LOG_CATEGORY_STATIC(BaseTrackLog, Display, All);

ABaseTrackedVehicle::ABaseTrackedVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

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
	CameraSceneComponent->SetUsingAbsoluteRotation(true);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraSceneComponent);

	TrackMovementComponent = CreateDefaultSubobject<UST_TrackMovementComponent>("MovementComponent");
}

void ABaseTrackedVehicle::MoveForward(const float Value)
{
	if (Controller != nullptr)
	{
		TrackMovementComponent->MoveForward(Value);
	}
}

void ABaseTrackedVehicle::MoveRight(const float Value)
{
	if (Controller != nullptr)
	{
		TrackMovementComponent->MoveRight(Value);
	}
}

void ABaseTrackedVehicle::RotateCamera(float Value)
{
	if (Controller != nullptr)
	{
		CameraSceneComponent->AddLocalRotation(FRotator{ 0, Value, 0 });
	}
}



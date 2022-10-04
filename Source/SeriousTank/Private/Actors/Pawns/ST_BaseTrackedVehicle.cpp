#include "Actors/Pawns/ST_BaseTrackedVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_TrackMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseTrackLog, Display, All);

AST_BaseTrackedVehicle::AST_BaseTrackedVehicle()
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

void AST_BaseTrackedVehicle::MoveForward(const float Value)
{
	if (Controller != nullptr)
	{
		TrackMovementComponent->MoveForward(Value);
	}
}

void AST_BaseTrackedVehicle::MoveRight(const float Value)
{
	if (Controller != nullptr)
	{
		TrackMovementComponent->MoveRight(Value);
	}
}

void AST_BaseTrackedVehicle::RotateCamera(float Value)
{
	if (Controller != nullptr)
	{
		CameraSceneComponent->AddLocalRotation(FRotator{ 0, Value, 0 });
	}
}

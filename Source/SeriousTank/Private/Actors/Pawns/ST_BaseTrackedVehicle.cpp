#include "Actors/Pawns/ST_BaseTrackedVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_TrackMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseTrackLog, Display, All);

AST_BaseTrackedVehicle::AST_BaseTrackedVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	SetRootComponent(BaseCollisionComponent);

	BaseStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	BaseStaticMeshComponent->SetupAttachment(RootComponent);

	CameraSceneComponent = CreateDefaultSubobject<USceneComponent>("CameraSceneComponent");
	CameraSceneComponent->SetupAttachment(RootComponent);
	CameraSceneComponent->SetUsingAbsoluteRotation(true);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraSceneComponent);

	TrackMovementComponent = CreateDefaultSubobject<UST_TrackMovementComponent>("MovementComponent");
	TrackMovementComponent->UpdatedComponent = RootComponent;
}

UPawnMovementComponent* AST_BaseTrackedVehicle::GetMovementComponent() const
{
	return TrackMovementComponent;
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

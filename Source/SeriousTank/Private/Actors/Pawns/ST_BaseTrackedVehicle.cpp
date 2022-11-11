#include "Actors/Pawns/ST_BaseTrackedVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_TrackMovementComponent.h"
#include "Components/ST_VehicleSoundsComponent.h"

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

	VehicleSoundComponent = CreateDefaultSubobject<UST_VehicleSoundsComponent>("AudioComponent");
	VehicleSoundComponent->SetupAttachment(RootComponent);

	TrackMovementComponent = CreateDefaultSubobject<UST_TrackMovementComponent>("MovementComponent");
	TrackMovementComponent->UpdatedComponent = RootComponent;
}

UPawnMovementComponent* AST_BaseTrackedVehicle::GetMovementComponent() const
{
	return TrackMovementComponent;
}

void AST_BaseTrackedVehicle::BeginPlay()
{
	Super::BeginPlay();

	if (TrackMovementComponent)
	{
		TrackMovementComponent->OnMovingTypeChanged.BindUObject(this, &ThisClass::OnMovingTypeChanged);
	}
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

void AST_BaseTrackedVehicle::OnMovingTypeChanged(EMovingType NewMovingType)
{
	if (VehicleSoundComponent)
	{
		VehicleSoundComponent->PlayMovingSound(NewMovingType);
	}
}
#include "Actors/Pawns/ST_BaseTrackedVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "Components/ST_TrackMovementComponent.h"
#include "Components/ST_VehicleSoundsComponent.h"
#include "Components/ST_ViewAreaBoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseTrackLog, Display, All);

AST_BaseTrackedVehicle::AST_BaseTrackedVehicle()
{
	BaseStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	BaseStaticMeshComponent->SetupAttachment(RootComponent);

	CameraSceneComponent = CreateDefaultSubobject<USceneComponent>("CameraSceneComponent");
	CameraSceneComponent->SetupAttachment(RootComponent);
	CameraSceneComponent->SetUsingAbsoluteRotation(true);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraSceneComponent);

	CameraViewAreaComponent = CreateDefaultSubobject<UST_ViewAreaBoxComponent>("CameraViewAreaComponent");
	CameraViewAreaComponent->SetupAttachment(CameraSceneComponent);
	CameraViewAreaComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

void AST_BaseTrackedVehicle::MoveForward(const FInputActionValue& ActionValue)
{
	if (Controller != nullptr)
	{
		TrackMovementComponent->MoveForward(ActionValue.Get<FInputActionValue::Axis1D>());
	}
}

void AST_BaseTrackedVehicle::MoveRight(const FInputActionValue& ActionValue)
{
	if (Controller != nullptr)
	{
		TrackMovementComponent->MoveRight(ActionValue.Get<FInputActionValue::Axis1D>());
	}
}

void AST_BaseTrackedVehicle::RotateCamera(const FInputActionValue& ActionValue)
{
	if (Controller != nullptr)
	{
		CameraSceneComponent->AddLocalRotation(FRotator{ 0, ActionValue.Get<FInputActionValue::Axis1D>(), 0 });
	}
}

void AST_BaseTrackedVehicle::OnMovingTypeChanged(EMovingType NewMovingType)
{
	if (VehicleSoundComponent)
	{
		VehicleSoundComponent->PlayMovingSound(NewMovingType);
	}
}

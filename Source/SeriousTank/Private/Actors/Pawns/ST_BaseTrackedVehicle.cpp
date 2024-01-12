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

	UpdateCameraViewArea();
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

void AST_BaseTrackedVehicle::UpdateCameraViewArea()
{
	FMinimalViewInfo DesiredView;
	CameraComponent->GetCameraView(0.f, DesiredView);

	FMatrix ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;
	UGameplayStatics::GetViewProjectionMatrix(DesiredView, ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);

	FPlane FrustumPlanes[4];
	ViewProjectionMatrix.GetFrustumLeftPlane(FrustumPlanes[0]);
	ViewProjectionMatrix.GetFrustumTopPlane(FrustumPlanes[1]);
	ViewProjectionMatrix.GetFrustumRightPlane(FrustumPlanes[2]);
	ViewProjectionMatrix.GetFrustumBottomPlane(FrustumPlanes[3]);

	FVector Intersections[4];
	for (int i = 0; i < 4; ++i)
	{
		// Finding intersection between 2 planes of frustum.
		FVector Intersection, Direction;
		const bool bIsIntersected = FMath::IntersectPlanes2(Intersection, Direction, FrustumPlanes[i], FrustumPlanes[(i + 1) % 4]);

		if (!bIsIntersected)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateCameraViewArea: Frustum planes are not intersected."));
			return;
		}

		// Calculate intersection point on 0z-axis.
		float ZeroZCoef = -Intersection.Z / Direction.Z;
		Intersections[i] = FVector{ Direction.X * ZeroZCoef + Intersection.X, Direction.Y * ZeroZCoef + Intersection.Y, 0.f };;
	}

	// Because of perspective view it's not possible to represent view area in box shape, so we also capture "blind areas" on the sides.
	const FVector ExpandedSideViewPoint = { Intersections[3].X, Intersections[0].Y, 0.f };

	const float HalfWidth = FVector::Dist(Intersections[1], Intersections[0]) / 2;
	const float HalfLength = FVector::Dist(Intersections[0], ExpandedSideViewPoint) / 2;

	const float ViewAreaHight = 150.f;
	CameraViewAreaComponent->SetWorldLocation(FVector{ ExpandedSideViewPoint.X + HalfLength, ExpandedSideViewPoint.Y + HalfWidth, 150.f });
	CameraViewAreaComponent->SetBoxExtent(FVector{ HalfLength, HalfWidth, ViewAreaHight });
}
#include "Actors/TrackedTank.h"

#include "Components/StaticMeshComponent.h"

ATrackedTank::ATrackedTank()
{
	TurretRotationSpeed = 30.f;

	TurretSceneComponent = CreateDefaultSubobject<USceneComponent>("TurretSceneComponent");
	TurretSceneComponent->SetupAttachment(VehicleSceneComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(TurretSceneComponent);

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMeshComponent");
	BarrelMeshComponent->SetupAttachment(TurretSceneComponent);
}

void ATrackedTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurretToCamera(DeltaTime);
}

void ATrackedTank::RotateTurretToCamera(float DeltaTime)
{
	const FRotator CameraRotator = CameraSceneComponent->GetComponentRotation();
	const FRotator TurretRotator = TurretSceneComponent->GetComponentRotation();
	
	if (CameraRotator.Yaw != TurretRotator.Yaw)
	{
		const float DifferenceAngle = CameraRotator.Yaw - TurretRotator.Yaw;
		const float MaxRotationAngle = TurretRotationSpeed * DeltaTime;

		const float RotationAngle = FMath::Min(MaxRotationAngle, FMath::Abs(DifferenceAngle)) * FMath::Sign(DifferenceAngle);
		TurretSceneComponent->AddLocalRotation(FRotator{ 0, RotationAngle, 0 });
	}


}

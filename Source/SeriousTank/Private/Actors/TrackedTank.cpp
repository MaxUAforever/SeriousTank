#include "Actors/TrackedTank.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Components/ST_WeaponsManagerComponent.h"

ATrackedTank::ATrackedTank()
{
	TurretRotationSpeed = 30.f;

	TurretSceneComponent = CreateDefaultSubobject<USceneComponent>("TurretSceneComponent");
	TurretSceneComponent->SetupAttachment(VehicleSceneComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(TurretSceneComponent);

	MainWeaponSocketComponent = CreateDefaultSubobject<UST_WeaponSocketComponent>("MainWeaponSocketComponent");
	MainWeaponSocketComponent->SetupAttachment(TurretSceneComponent);

	SecondWeaponSocketComponent = CreateDefaultSubobject<UST_WeaponSocketComponent>("SecondWeaponSocketComponent");
	SecondWeaponSocketComponent->SetupAttachment(TurretSceneComponent);

	WeaponsManagerComponent = CreateDefaultSubobject<UST_WeaponsManagerComponent>("WeaponsManagerComponent");
}

void ATrackedTank::BeginPlay()
{
	Super::BeginPlay();
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

void ATrackedTank::StartFire()
{
	WeaponsManagerComponent->StartFire();
}

void ATrackedTank::StopFire()
{
	WeaponsManagerComponent->StopFire();
}

void ATrackedTank::SwitchToFirstWeapon() 
{
	WeaponsManagerComponent->SwitchWeapon(0);
}

void ATrackedTank::SwitchToSecondWeapon() 
{
	WeaponsManagerComponent->SwitchWeapon(1);
}
#include "Actors/Pawns/ST_TrackedTank.h"

#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Components/Weapons/ST_VehicleWeaponManagerComponent.h"

AST_TrackedTank::AST_TrackedTank()
{
	TurretRotationSpeed = 30.f;

	TurretSceneComponent = CreateDefaultSubobject<USceneComponent>("TurretSceneComponent");
	TurretSceneComponent->SetupAttachment(RootComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(TurretSceneComponent);

	MainWeaponSocketComponent = CreateDefaultSubobject<UST_WeaponSocketComponent>("MainWeaponSocketComponent");
	MainWeaponSocketComponent->SetupAttachment(TurretSceneComponent);

	TurretRotationSound = CreateDefaultSubobject<UAudioComponent>("TurretRotationAudio");
	TurretRotationSound->SetupAttachment(TurretSceneComponent);

	SecondWeaponSocketComponent = CreateDefaultSubobject<UST_WeaponSocketComponent>("SecondWeaponSocketComponent");
	SecondWeaponSocketComponent->SetupAttachment(TurretSceneComponent);

	WeaponManagerComponent = CreateDefaultSubobject<UST_VehicleWeaponManagerComponent>("WeaponManagerComponent");
}

void AST_TrackedTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurretToCamera(DeltaTime);
}

void AST_TrackedTank::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	SetActorTickEnabled(IsValid(GetController()));

	if (IsValid(TurretSceneComponent) && IsValid(CameraSceneComponent))
	{
		FRotator TargetRotator = CameraSceneComponent->GetComponentRotation();
		TargetRotator.Yaw = TurretSceneComponent->GetComponentRotation().Yaw;

		CameraSceneComponent->SetWorldRotation(TargetRotator);
	}
}

void AST_TrackedTank::RotateTurretToCamera(float DeltaTime)
{
	const FRotator CameraRotator = CameraSceneComponent->GetComponentRotation();
	const FRotator TurretRotator = TurretSceneComponent->GetComponentRotation();

	if (!FMath::IsNearlyEqual(CameraRotator.Yaw, TurretRotator.Yaw, 0.0001f))
	{
		const float AbsoluteDifferenceAngle = CameraRotator.Yaw - TurretRotator.Yaw;
		const float DifferenceAngle = FMath::Abs(AbsoluteDifferenceAngle) > 180.f ? ((FMath::Abs(AbsoluteDifferenceAngle) - 180.f) * (-1 * FMath::Sign(AbsoluteDifferenceAngle))) : AbsoluteDifferenceAngle;
		const float MaxRotationAngle = TurretRotationSpeed * DeltaTime * FMath::Sign(DifferenceAngle);

		const float RotationAngle = FMath::Abs(DifferenceAngle) < FMath::Abs(MaxRotationAngle) ? DifferenceAngle : MaxRotationAngle;
		TurretSceneComponent->AddLocalRotation(FRotator{ 0, RotationAngle, 0 });

		if (!TurretRotationSound->IsActive())
		{
			TurretRotationSound->Play();
		}
	}
	else if (TurretRotationSound->IsActive())
	{
		TurretRotationSound->Stop();
	}
}

void AST_TrackedTank::StartFire()
{
	WeaponManagerComponent->StartFire();
}

void AST_TrackedTank::StopFire()
{
	WeaponManagerComponent->StopFire();
}

void AST_TrackedTank::SwitchToFirstWeapon()
{
    WeaponManagerComponent->StartSwitchingWeapon(0);
}

void AST_TrackedTank::SwitchToSecondWeapon()
{
	WeaponManagerComponent->StartSwitchingWeapon(1);
}

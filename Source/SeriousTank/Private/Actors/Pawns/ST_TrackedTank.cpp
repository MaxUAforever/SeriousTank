#include "Actors/Pawns/ST_TrackedTank.h"

#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Components/ST_WeaponsManagerComponent.h"

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

	WeaponsManagerComponent = CreateDefaultSubobject<UST_WeaponsManagerComponent>("WeaponsManagerComponent");
}

void AST_TrackedTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurretToCamera(DeltaTime);
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

		UE_LOG(LogTemp, Warning, TEXT("DifferenceAngle: %f = CameraRotator.Yaw: %f - TurretRotator.Yaw: %f"), DifferenceAngle, CameraRotator.Yaw, TurretRotator.Yaw);
	}
	else if (TurretRotationSound->IsActive())
	{
		TurretRotationSound->Stop();
	}
}

void AST_TrackedTank::StartFire()
{
	WeaponsManagerComponent->StartFire();
}

void AST_TrackedTank::StopFire()
{
	WeaponsManagerComponent->StopFire();
}

void AST_TrackedTank::SwitchToFirstWeapon()
{
    Super::SwitchToFirstWeapon();
    
	WeaponsManagerComponent->SwitchWeapon(0);
}

void AST_TrackedTank::SwitchToSecondWeapon()
{
    Super::SwitchToSecondWeapon();
    
	WeaponsManagerComponent->SwitchWeapon(1);
}

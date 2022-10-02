#include "Actors/Weapons/BarrelWeapon.h"

#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

ABarrelWeapon::ABarrelWeapon()
{
	ReloadingTime = 5.f;

	IsFireForced = false;
	IsReloading = false;
}

void ABarrelWeapon::StartFire()
{
	IsFireForced = true;
	Shoot();
}

void ABarrelWeapon::StopFire()
{
	IsFireForced = false;
}

void ABarrelWeapon::Shoot()
{
	if (IsReloading)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->SpawnActor<ABaseProjectile>(ProjectileClass, ShootingArrowComponent->GetComponentTransform());
	}

	StartReloading();
}

void ABarrelWeapon::StartReloading()
{
	IsReloading = true;

	FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::StopReloading);
	GetWorldTimerManager().SetTimer(ReloadTimerHandler, FireTimerDelegate, ReloadingTime, false);
}

void ABarrelWeapon::StopReloading()
{
	IsReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandler);

	if (IsFireForced)
	{
		Shoot();
	}
}

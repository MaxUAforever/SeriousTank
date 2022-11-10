#include "Actors/Weapons/BarrelWeapon.h"

#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

ABarrelWeapon::ABarrelWeapon()
{
	ReloadingTime = 5.f;

	IsFireForced = false;
	IsWeaponReloading = false;
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

bool ABarrelWeapon::IsReloading() const
{
	return IsWeaponReloading;
}

float ABarrelWeapon::GetReloadingRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(ReloadTimerHandler);
}

void ABarrelWeapon::Shoot()
{
	if (IsWeaponReloading)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		World->SpawnActor<AST_BaseProjectile>(ProjectileClass, ShootingArrowComponent->GetComponentTransform(), SpawnParameters);
	}

	StartReloading();
}

void ABarrelWeapon::StartReloading()
{
	IsWeaponReloading = true;

	FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::StopReloading);
	GetWorldTimerManager().SetTimer(ReloadTimerHandler, FireTimerDelegate, ReloadingTime, false);

	OnReloadingStarted.ExecuteIfBound();
}

void ABarrelWeapon::StopReloading()
{
	IsWeaponReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandler);
	
	if (IsFireForced)
	{
		Shoot();
	}
}

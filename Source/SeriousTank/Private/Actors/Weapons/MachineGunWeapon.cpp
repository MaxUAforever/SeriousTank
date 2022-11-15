#include "Actors/Weapons/MachineGunWeapon.h"

#include "TimerManager.h"

AMachineGunWeapon::AMachineGunWeapon()
{
	MaxClipAmmoCount = 10;
	BurstShootingPause = 0.2f;

	CurrentClipAmmoCount = MaxClipAmmoCount;
}

void AMachineGunWeapon::StartShooting()
{
	Shoot();

	if (!GetWorldTimerManager().IsTimerActive(BurstShootingTimerHandler))
	{
		FTimerDelegate ShootingDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnBurstPauseEnded);
		GetWorldTimerManager().SetTimer(BurstShootingTimerHandler, ShootingDelegate, BurstShootingPause, true);
	}
}

void AMachineGunWeapon::FinishReloading()
{
	CurrentClipAmmoCount = MaxClipAmmoCount;

	Super::FinishReloading();
}

void AMachineGunWeapon::Shoot()
{
	if (IsReloading() || bIsInBurstPause)
	{
		return;
	}

	bIsInBurstPause = true;

	Super::Shoot();
	if (--CurrentClipAmmoCount == 0)
	{
		StopShooting();
		StartReloading();
	}
}

void AMachineGunWeapon::OnBurstPauseEnded()
{
	bIsInBurstPause = false;
	if (IsFireForced())
	{
		Shoot();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(BurstShootingTimerHandler);
	}
}
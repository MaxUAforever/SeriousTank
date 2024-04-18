#include "Actors/Weapons/ST_MachineGunWeapon.h"

#include "TimerManager.h"

void AST_MachineGunWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentClipAmmoCount = MaxClipAmmoCount < TotalAmmoCount ? MaxClipAmmoCount : TotalAmmoCount;
}

void AST_MachineGunWeapon::StartShooting()
{
	Shoot();

	if (!GetWorldTimerManager().IsTimerActive(BurstShootingTimerHandler))
	{
		FTimerDelegate ShootingDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnBurstPauseEnded);
		GetWorldTimerManager().SetTimer(BurstShootingTimerHandler, ShootingDelegate, BurstShootingPause, true);
	}
}

bool AST_MachineGunWeapon::CanReload() const
{
	return CurrentClipAmmoCount < MaxClipAmmoCount && TotalAmmoCount > CurrentClipAmmoCount;
}

bool AST_MachineGunWeapon::IsReloadingNeeded() const
{
	return CurrentClipAmmoCount == 0 && TotalAmmoCount > 0;
}

void AST_MachineGunWeapon::FinishReloading()
{
    CurrentClipAmmoCount = MaxClipAmmoCount < TotalAmmoCount ? MaxClipAmmoCount : TotalAmmoCount;
	OnClipAmmoCountChanged.Broadcast(CurrentClipAmmoCount);

	Super::FinishReloading();
}

void AST_MachineGunWeapon::Shoot()
{
	if (!CanShoot() || bIsInBurstPause)
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

	OnShootDone.ExecuteIfBound();
	OnClipAmmoCountChanged.Broadcast(CurrentClipAmmoCount);
}

void AST_MachineGunWeapon::OnBurstPauseEnded()
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

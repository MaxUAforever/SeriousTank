#include "Actors/Weapons/ST_MachineGunWeapon.h"

#include "TimerManager.h"

void AST_MachineGunWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentClipAmmoCount = MaxClipAmmoCount < GetTotalAmmoCount() ? MaxClipAmmoCount : GetTotalAmmoCount();
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
	return CurrentClipAmmoCount < MaxClipAmmoCount && GetTotalAmmoCount() > CurrentClipAmmoCount;
}

bool AST_MachineGunWeapon::IsReloadingNeeded() const
{
	return CurrentClipAmmoCount == 0 && GetTotalAmmoCount() > 0;
}

void AST_MachineGunWeapon::OnReloadingCompleted()
{
    CurrentClipAmmoCount = MaxClipAmmoCount < GetTotalAmmoCount() ? MaxClipAmmoCount : GetTotalAmmoCount();
	OnClipAmmoCountChanged.Broadcast(CurrentClipAmmoCount);
}

void AST_MachineGunWeapon::Shoot()
{
	if (!CanShoot() || bIsInBurstPause)
	{
		return;
	}

	bIsInBurstPause = true;
	CurrentClipAmmoCount -= 1;

	Super::Shoot();

	OnClipAmmoCountChanged.Broadcast(CurrentClipAmmoCount);
}

void AST_MachineGunWeapon::OnBurstPauseEnded()
{
	bIsInBurstPause = false;
	if (IsFireForced())
	{
		Shoot();

		if (IsReloadingNeeded())
		{
			StopShooting();
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(BurstShootingTimerHandler);
	}
}

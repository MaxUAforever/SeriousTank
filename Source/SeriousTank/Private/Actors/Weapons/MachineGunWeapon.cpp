#include "Actors/Weapons/MachineGunWeapon.h"

#include "TimerManager.h"

void AMachineGunWeapon::BeginPlay()
{
    Super::BeginPlay();
    
	CurrentClipAmmoCount = MaxClipAmmoCount < TotalAmmoCount ? MaxClipAmmoCount : TotalAmmoCount;
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
    OnClipAmmoCountChanged.Broadcast(CurrentClipAmmoCount);
    
	Super::FinishReloading();
}

void AMachineGunWeapon::Shoot()
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
    
    OnClipAmmoCountChanged.Broadcast(CurrentClipAmmoCount);
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

#include "Actors/Weapons/BaseWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	ShootingArrowComponent = CreateDefaultSubobject<UArrowComponent>("ShootingArrowComponent");
	ShootingArrowComponent->SetupAttachment(RootComponent);

	bIsFireForced = false;
	bIsWeaponReloading = false;
	
	ReloadingTime = 5.f;
}

float ABaseWeapon::GetReloadingRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(ReloadTimerHandler);
}

void ABaseWeapon::StartFire()
{
	bIsFireForced = true;
	StartShooting();
}

void ABaseWeapon::StopFire()
{
	bIsFireForced = false;
	StopShooting();
}

void ABaseWeapon::StartReloading()
{
	if (bIsWeaponReloading)
	{
		return;
	}

	bIsWeaponReloading = true;

	FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::StopReloading);
	GetWorldTimerManager().SetTimer(ReloadTimerHandler, FireTimerDelegate, ReloadingTime, false);

	OnReloadingStarted.ExecuteIfBound();
}

void ABaseWeapon::StopReloading()
{
	bIsWeaponReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandler);

	FinishReloading();
}


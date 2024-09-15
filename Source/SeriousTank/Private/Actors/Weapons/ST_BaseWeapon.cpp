#include "Actors/Weapons/ST_BaseWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ST_CoreTypes.h"
#include "TimerManager.h"

AST_BaseWeapon::AST_BaseWeapon()
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
	TotalAmmoCount = 50;
}

float AST_BaseWeapon::GetReloadingRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(ReloadTimerHandler);
}

void AST_BaseWeapon::SetTotalAmmoCount(int32 NewAmmoCount)
{
	if (NewAmmoCount >= 0)
	{
		int32 OldAmmoCount = TotalAmmoCount;
		TotalAmmoCount = NewAmmoCount;

		if (OldAmmoCount == 0 && NewAmmoCount > 0)
		{
			StartReloading();
		}
	}
}

void AST_BaseWeapon::StartFire()
{
	if (bIsEnabled)
	{
		bIsFireForced = true;
		StartShooting();
	}
}

void AST_BaseWeapon::StopFire()
{
	bIsFireForced = false;
	StopShooting();
}

void AST_BaseWeapon::AttachToParentComponent(USceneComponent* InParentComponent, FName SocketName, bool bInShouldBeDestroyedWithActor)
{
	if (InParentComponent)
	{
		AttachToComponent(InParentComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		
		AActor* OwnerActor = InParentComponent->GetOwner();
		SetOwner(OwnerActor);

		OwnerActor->OnDestroyed.AddDynamic(this, &ThisClass::OnParentDestroyed);
		bShouldBeDestroyedWithActor = bInShouldBeDestroyedWithActor;
	}
}

void AST_BaseWeapon::SetEnabled(bool bInIsEnabled)
{
	bIsEnabled = bInIsEnabled;
	if (!bIsEnabled)
	{
		StopFire();
	}

	OnSetWeaponEnabled(bInIsEnabled);
}

void AST_BaseWeapon::StartReloading()
{
	if (bIsWeaponReloading || TotalAmmoCount <= 0)
	{
		return;
	}

	bIsWeaponReloading = true;

	FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::StopReloading);
	GetWorldTimerManager().SetTimer(ReloadTimerHandler, FireTimerDelegate, ReloadingTime, false);

	OnReloadingStarted.Broadcast();
}

void AST_BaseWeapon::InterruptReloading()
{
	bIsWeaponReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandler);
}

void AST_BaseWeapon::ForceReload()
{
    if (CanReload())
    {
        StartReloading();
    }
}

void AST_BaseWeapon::StopReloading()
{
	bIsWeaponReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandler);

	FinishReloading();
}

void AST_BaseWeapon::SetHidden(bool bIsHidden)
{
	if (WeaponOwnerType == EWeaponOwnerType::Vehicle)
	{
		return;
	}

	//SetWeaponEnabled(!bIsHidden);

	SceneComponent->SetVisibility(!bIsHidden, true);
}

bool AST_BaseWeapon::CanShoot() const
{
	return bIsEnabled && !bIsWeaponReloading && TotalAmmoCount > 0;
}

void AST_BaseWeapon::OnParentDestroyed(AActor* DestroyedOwner)
{
	if (bShouldBeDestroyedWithActor)
	{
		Destroy();
	}
	else
	{
		DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}
}


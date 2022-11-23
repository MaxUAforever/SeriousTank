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
	TotalAmmoCount = 50;
}

float ABaseWeapon::GetReloadingRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(ReloadTimerHandler);
}

void ABaseWeapon::SetTotalAmmoCount(int32 NewAmmoCount)
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

void ABaseWeapon::AttachToVehicleComponent(USceneComponent* ParentVehicleComponent)
{
    if (ParentVehicleComponent)
    {
        AttachToComponent(ParentVehicleComponent, FAttachmentTransformRules::KeepWorldTransform);
        
        AActor* Owner = ParentVehicleComponent->GetOwner();
        Owner->OnDestroyed.AddDynamic(this, &ThisClass::OnVehicleDestroyed);
        SetOwner(Owner);
    }
}

void ABaseWeapon::StartReloading()
{
	if (bIsWeaponReloading || TotalAmmoCount <= 0)
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

bool ABaseWeapon::CanShoot() const
{
	return !bIsWeaponReloading && TotalAmmoCount > 0;
}

void ABaseWeapon::OnVehicleDestroyed(AActor* DestroyedOwnerVehicle)
{
    Destroy();
}

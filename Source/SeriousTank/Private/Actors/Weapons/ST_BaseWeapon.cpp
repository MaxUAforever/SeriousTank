#include "Actors/Weapons/ST_BaseWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ST_CoreTypes.h"
#include "Engine/World.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractionComponent.h"
#include "Subsystems/PlayerInteractionSubsystem/Actions/ST_WeaponPickUpAction.h"
#include "TimerManager.h"

AST_BaseWeapon::AST_BaseWeapon()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	ShootingArrowComponent = CreateDefaultSubobject<UArrowComponent>("ShootingArrowComponent");
	ShootingArrowComponent->SetupAttachment(RootComponent);

	PickUpInteractionComponent = CreateDefaultSubobject<UInteractionComponent>("PickUpInteractionComponent");
	PickUpInteractionComponent->SetupAttachment(RootComponent);
	PickUpInteractionComponent->SetActionClass(UST_WeaponPickUpAction::StaticClass());

	bIsFireForced = false;
	bIsWeaponReloading = false;

	ReloadingTime = 5.f;
	TotalAmmoCount = 50;
	MaxAmmoCount = 90;
}

void AST_BaseWeapon::SetTotalAmmoCount(int32 NewAmmoCount)
{
	if (TotalAmmoCount == NewAmmoCount)
	{
		return;
	}

	const float OldAmmoCount = TotalAmmoCount;
	TotalAmmoCount = FMath::Clamp(NewAmmoCount, 0, MaxAmmoCount);

	OnAmmoCountChangedDelegate.Broadcast(OldAmmoCount, TotalAmmoCount);
}

void AST_BaseWeapon::AddAmmo(int32 AddedAmmoCount)
{
	SetTotalAmmoCount(TotalAmmoCount + AddedAmmoCount);
}

void AST_BaseWeapon::SetWeaponEquipped(bool bInIsEquipped)
{
	MeshComponent->SetSimulatePhysics(!bInIsEquipped);
	PickUpInteractionComponent->SetIsInteractionComponentActive(!bInIsEquipped);
}

bool AST_BaseWeapon::StartFire()
{
	if (bIsEnabled)
	{
		bIsFireForced = true;
		StartShooting();
	}

	return bIsEnabled;
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
		AddActorLocalOffset(AttachmentOffset.GetLocation());
		AddActorLocalRotation(AttachmentOffset.GetRotation());

		AActor* OwnerActor = InParentComponent->GetOwner();
		SetOwner(OwnerActor);

		OwnerActor->OnDestroyed.AddDynamic(this, &ThisClass::OnParentDestroyed);
		bShouldBeDestroyedWithActor = bInShouldBeDestroyedWithActor;
	}
}

void AST_BaseWeapon::DetachFromParentComponent()
{
	AActor* ParentActor = GetAttachParentActor();
	if (IsValid(ParentActor))
	{
		ParentActor->OnDestroyed.RemoveAll(this);
		MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
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

bool AST_BaseWeapon::StartReloading()
{
	if (!CanReload())
	{
		return false;
	}

	bIsWeaponReloading = true;

	OnReloadingStarted();
	OnReloadingStartedDelegate.Broadcast();

	return true;
}

void AST_BaseWeapon::CompleteReloading()
{
	if (!bIsWeaponReloading)
	{
		return;
	}

	bIsWeaponReloading = false;
	OnReloadingCompleted();

	if (IsFireForced())
	{
		StartShooting();
	}
}

void AST_BaseWeapon::InterruptReloading()
{
	if (bIsWeaponReloading)
	{
		bIsWeaponReloading = false;

		OnReloadingInterrupted();
	}
}

void AST_BaseWeapon::SetHidden(bool bIsHidden)
{
	if (WeaponOwnerType == EWeaponOwnerType::Vehicle)
	{
		return;
	}

	GetRootComponent()->SetVisibility(!bIsHidden, true);
}

bool AST_BaseWeapon::CanShoot() const
{
	return bIsEnabled && !bIsWeaponReloading && TotalAmmoCount > 0;
}

bool AST_BaseWeapon::CanReload() const
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


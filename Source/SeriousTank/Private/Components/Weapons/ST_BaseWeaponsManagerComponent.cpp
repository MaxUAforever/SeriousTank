#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ST_GameInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

UST_BaseWeaponsManagerComponent::UST_BaseWeaponsManagerComponent()
{
	CurrentWeaponIndex = 0;
}

void UST_BaseWeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* PawnOwner = Cast<APawn>(GetOwner()))
	{
		PawnOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnControllerChanged);
	}

//#if WITH_EDITOR
	if (OverriddenFirstWeapon)
	{
		if (AST_BaseWeapon* Weapon = GetWorld()->SpawnActor<AST_BaseWeapon>(OverriddenFirstWeapon))
		{
			AddWeapon(Weapon);
		}
	}

	if (OverriddenSecondWeapon)
	{
		if (AST_BaseWeapon* Weapon = GetWorld()->SpawnActor<AST_BaseWeapon>(OverriddenSecondWeapon))
		{
			AddWeapon(Weapon);
		}
	}
//#endif
}

void UST_BaseWeaponsManagerComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	InterruptReloading();
}

bool UST_BaseWeaponsManagerComponent::StartFire()
{
	AST_BaseWeapon* CurrentWeapon = GetCurrentWeapon();
	if (!IsValid(CurrentWeapon))
	{
		return false;
	}

	CurrentWeapon->StartFire();
	return true;
}

void UST_BaseWeaponsManagerComponent::StopFire()
{
	AST_BaseWeapon* CurrentWeapon = GetCurrentWeapon();
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopFire();
	}
}

void UST_BaseWeaponsManagerComponent::ReloadCurrentWeapon()
{
	UWorld* World = GetWorld();
	AST_BaseWeapon* CurrentWeapon = GetCurrentWeapon();
	if (!IsValid(CurrentWeapon) || !IsValid(World))
	{
		return;
	}

	bool bSuccessfullyStarted = CurrentWeapon->StartReloading();
	if (!bSuccessfullyStarted)
	{
		return;
	}

	OnWeaponReloadingStartedDelegate.Broadcast(CurrentWeaponIndex, CurrentWeapon);

	const float WeponReloadingTime = CurrentWeapon->GetTotalReloadingTime();
	if (WeponReloadingTime > 0)
	{
		World->GetTimerManager().SetTimer(WeaponsInfo[CurrentWeaponIndex].ReloadingTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::FinishReloading, CurrentWeaponIndex), WeponReloadingTime, false);
	}
	else
	{
		FinishReloading(CurrentWeaponIndex);
	}
}

void UST_BaseWeaponsManagerComponent::InterruptReloading()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	for (FWeaponAdditionalInfo& WeaponInfo : WeaponsInfo)
	{
		if (!WeaponInfo.ReloadingTimerHandle.IsValid())
		{
			World->GetTimerManager().ClearTimer(WeaponInfo.ReloadingTimerHandle);
		}
	}

	int32 Index = 0;
	for (AST_BaseWeapon* Weapon : Weapons)
	{
		if (IsValid(Weapon))
		{
			Weapon->InterruptReloading();

			OnWeaponReloadingFinishedDelegate.Broadcast(Index++, Weapon);
		}
	}
}

bool UST_BaseWeaponsManagerComponent::IsReloadingWeapon(int32 WeaponIndex) const
{
	return GetWeaponReloadingTime(WeaponIndex) > 0.f;
}

float UST_BaseWeaponsManagerComponent::GetWeaponReloadingTime(int32 WeaponIndex) const
{
	UWorld* World = GetWorld();
	const FWeaponAdditionalInfo* WeaponInfo = GetWeaponInfo(WeaponIndex);
	if (!IsValid(World))
	{
		return 0.f;
	}

	if (WeaponInfo == nullptr || !World->GetTimerManager().IsTimerActive(WeaponInfo->ReloadingTimerHandle))
	{
		return 0.f;
	}

	return World->GetTimerManager().GetTimerRemaining(WeaponInfo->ReloadingTimerHandle);
}

float UST_BaseWeaponsManagerComponent::GetWeaponTotalReloadingTime(int32 WeaponIndex) const
{
	AST_BaseWeapon* Weapon = GetWeapon(WeaponIndex);
	if (!IsValid(Weapon))
	{
		return 0.f;
	}

	return Weapon->GetTotalReloadingTime();
}

bool UST_BaseWeaponsManagerComponent::StartSwitchingWeapon(int32 WeaponIndex)
{
	if (IsWeaponSwitching())
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return false;
	}

	if (!Weapons.IsValidIndex(WeaponIndex) || !Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		return false;
	}

	AST_BaseWeapon* CurrentWeapon = Weapons[CurrentWeaponIndex];
	AST_BaseWeapon* NewWeapon = Weapons[WeaponIndex];
	if (!IsValid(CurrentWeapon) || !IsValid(NewWeapon))
	{
		return false;
	}

	StopFire();
	
	bIsWeaponSwitching = true;
	CurrentWeapon->SetEnabled(false);
	
	OnWeaponSwitchingStarted(CurrentWeaponIndex, WeaponIndex);
	OnWeaponSwitchingStartedDelegate.Broadcast(CurrentWeaponIndex, WeaponIndex);
	
	CurrentWeaponIndex = WeaponIndex;

	if (WeaponSwitchingTime > 0)
	{
		World->GetTimerManager().SetTimer(WeaponSwitchingTimerHandle, this, &ThisClass::CompleteWeaponSwitching, WeaponSwitchingTime, false);
	}
	else
	{
		CompleteWeaponSwitching();
	}

	return true;
}

void UST_BaseWeaponsManagerComponent::FinishReloading(int32 WeaponIndex)
{
	AST_BaseWeapon* Weapon = GetWeapon(WeaponIndex);
	if (!IsValid(Weapon))
	{
		return;
	}
		
	Weapon->CompleteReloading();
	OnWeaponReloadingFinishedDelegate.Broadcast(WeaponIndex, Weapon);
}

AST_BaseWeapon* UST_BaseWeaponsManagerComponent::GetCurrentWeapon() const
{
	return GetWeapon(CurrentWeaponIndex);
}

AST_BaseWeapon* UST_BaseWeaponsManagerComponent::GetWeapon(int32 WeaponIndex) const
{
	return Weapons.IsValidIndex(WeaponIndex) ? Weapons[WeaponIndex] : nullptr;
}

bool UST_BaseWeaponsManagerComponent::AddWeapon(AST_BaseWeapon* NewWeapon)
{
    if (!NewWeapon)
    {
        return false;
    }
    
	if (Weapons.Num() >= GetMaxWeaponsAmount())
	{
		return false;
	}

    NewWeapon->SetOwner(GetOwner());
	NewWeapon->SetActorEnableCollision(false);

	if (Weapons.Num() == 0)
	{
		NewWeapon->SetEnabled(true);
	}

    int32 Index = Weapons.Add(NewWeapon);
	WeaponsInfo.EmplaceAt(Index);

	NewWeapon->SetWeaponEquipped(true);
	NewWeapon->OnAmmoCountChangedDelegate.AddUObject(this, &ThisClass::OnAmmoCountChanged, Index);

	OnWeaponAdded(NewWeapon);
	OnWeaponAddedDelegate.Broadcast(Index, NewWeapon);

	return true;
}

void UST_BaseWeaponsManagerComponent::RemoveWeapon(int32 WeaponIndex)
{
	if (!Weapons.IsValidIndex(WeaponIndex))
	{
		return;
	}

	if (!CanChangeWeapons())
	{
		return;
	}

	AST_BaseWeapon* RemovedWeapon = GetWeapon(WeaponIndex);
	if (!IsValid(RemovedWeapon))
	{
		return;
	}

	OnPreWeaponRemoved(WeaponIndex);
	OnPreWeaponRemovedDelegate.Broadcast(WeaponIndex, RemovedWeapon);

	RemovedWeapon->OnAmmoCountChangedDelegate.RemoveAll(this);
	RemovedWeapon->SetActorEnableCollision(true);
	RemovedWeapon->SetEnabled(false);
	RemovedWeapon->SetWeaponEquipped(false);

	Weapons.RemoveAt(WeaponIndex);
	WeaponsInfo.RemoveAt(WeaponIndex);
}

void UST_BaseWeaponsManagerComponent::CompleteWeaponSwitching()
{
	if (!Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		return;
	}

	AST_BaseWeapon* CurrentWeapon = Weapons[CurrentWeaponIndex];
	if (!IsValid(CurrentWeapon))
	{
		return;
	}

	bIsWeaponSwitching = false;
	CurrentWeapon->SetEnabled(true);

	OnWeaponSwitchingCompleted();
	OnWeaponSwitchingCompletedDelegate.Broadcast();
}

void UST_BaseWeaponsManagerComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (NewController)
	{
		OnOwnerPawnPossessed(NewController);
	}
	else
	{
		OnOwnerPawnUnPossessed(OldController);
	}
}

void UST_BaseWeaponsManagerComponent::OnOwnerPawnPossessed(AController* NewController)
{
	AST_BaseWeapon* CurrentWeapon = GetCurrentWeapon();
	if (IsValid(CurrentWeapon) && CurrentWeapon->IsReloadingNeeded())
	{
		ReloadCurrentWeapon();
	}
}

void UST_BaseWeaponsManagerComponent::OnOwnerPawnUnPossessed(AController* OldController)
{
	InterruptReloading();
}

void UST_BaseWeaponsManagerComponent::OnAmmoCountChanged(int32 OldAmmoCount, int32 NewAmmoCount, int32 WeaponIndex)
{
	AST_BaseWeapon* Weapon = GetWeapon(WeaponIndex);
	if (!IsValid(Weapon))
	{
		return;
	}
	
	
	if (NewAmmoCount == 0)
	{
		OnWeaponOutOfAmmoDelegate.Broadcast(WeaponIndex, Weapon);
	}
	else if (NewAmmoCount > OldAmmoCount)
	{
		OnAmmoRefilledDelegate.Broadcast(WeaponIndex, Weapon);
	}

	if (WeaponIndex == GetCurrentWeaponIndex() && NewAmmoCount > 0 && Weapon->IsReloadingNeeded())
	{
		ReloadCurrentWeapon();
	}
}

const FWeaponAdditionalInfo* UST_BaseWeaponsManagerComponent::GetWeaponInfo(int32 WeaponIndex) const
{
	if (!WeaponsInfo.IsValidIndex(WeaponIndex) || GetWeapon(WeaponIndex) == nullptr)
	{
		return nullptr;
	}

	return &WeaponsInfo[WeaponIndex];
}

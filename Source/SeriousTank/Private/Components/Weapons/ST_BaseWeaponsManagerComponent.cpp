#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ST_GameInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


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

#if WITH_EDITOR
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
#endif
}

void UST_BaseWeaponsManagerComponent::StartFire()
{
	if (Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		Weapons[CurrentWeaponIndex]->StartFire();
	}
}

void UST_BaseWeaponsManagerComponent::StopFire()
{
	if (Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		Weapons[CurrentWeaponIndex]->StopFire();
	}
}

void UST_BaseWeaponsManagerComponent::Reload()
{
    if (Weapons.IsValidIndex(CurrentWeaponIndex))
    {
        Weapons[CurrentWeaponIndex]->ForceReload();
    }
}

void UST_BaseWeaponsManagerComponent::InterruptReloading()
{
	for (AST_BaseWeapon* Weapon : Weapons)
	{
		Weapon->InterruptReloading();
	}
}

bool UST_BaseWeaponsManagerComponent::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num() || Weapons[WeaponIndex] == nullptr)
	{
		return false;
	}

	StopFire();

	Weapons[CurrentWeaponIndex]->SetEnabled(false);
	Weapons[WeaponIndex]->SetEnabled(true);

	OnWeaponSwitched(CurrentWeaponIndex, WeaponIndex);
	OnWeaponSwitchedDelegate.Broadcast(CurrentWeaponIndex, WeaponIndex);

	CurrentWeaponIndex = WeaponIndex;

	return true;
}

AST_BaseWeapon* UST_BaseWeaponsManagerComponent::GetCurrentWeapon() const
{
	return GetWeapon(CurrentWeaponIndex);
}

AST_BaseWeapon* UST_BaseWeaponsManagerComponent::GetWeapon(int32 WeaponIndex) const
{
	return (WeaponIndex >= 0 && WeaponIndex < Weapons.Num()) ? Weapons[WeaponIndex] : nullptr;
}

void UST_BaseWeaponsManagerComponent::AddWeapon(AST_BaseWeapon* NewWeapon)
{
    if (!NewWeapon)
    {
        return;
    }
    
    NewWeapon->SetOwner(GetOwner());
	NewWeapon->SetActorEnableCollision(false);

	if (Weapons.Num() == 0)
	{
		NewWeapon->SetEnabled(true);
	}

    Weapons.Add(NewWeapon);

    OnWeaponAdded.Broadcast(Weapons.Num() - 1, NewWeapon);
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

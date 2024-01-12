#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ST_GameInstance.h"
#include "GameFramework/Actor.h"


UST_BaseWeaponsManagerComponent::UST_BaseWeaponsManagerComponent()
{
	CurrentWeaponIndex = 0;
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

bool UST_BaseWeaponsManagerComponent::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num() || Weapons[WeaponIndex] == nullptr)
	{
		return false;
	}

	StopFire();

	Weapons[CurrentWeaponIndex]->SetWeaponEnabled(false);
	Weapons[WeaponIndex]->SetWeaponEnabled(true);

	OnWeaponSwitched(CurrentWeaponIndex, WeaponIndex);

	CurrentWeaponIndex = WeaponIndex;

	return true;
}

AST_BaseWeapon* UST_BaseWeaponsManagerComponent::GetCurrentWeapon() const
{
	return GetWeapon(CurrentWeaponIndex);
}

AST_BaseWeapon* UST_BaseWeaponsManagerComponent::GetWeapon(int32 WeaponIndex) const
{
	return (WeaponIndex >= 0 && WeaponIndex < Weapons.Num()) ? Weapons[CurrentWeaponIndex] : nullptr;
}



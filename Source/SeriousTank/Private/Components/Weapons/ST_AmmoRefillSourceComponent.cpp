#include "Components/Weapons/ST_AmmoRefillSourceComponent.h"

bool UST_AmmoRefillSourceComponent::HasAvailableAmmo(TSubclassOf<AST_BaseWeapon> WeaponClass)
{
	const int32* WeaponAmmoAmount = AmmoLimits.Find(WeaponClass);
	
	return !bAreWeaponsLimited || (WeaponAmmoAmount != nullptr && (!bIsAmmoLimited || *WeaponAmmoAmount > 0));
}

int32 UST_AmmoRefillSourceComponent::TakeAvailableAmmo(TSubclassOf<AST_BaseWeapon> WeaponClass, int32 RequestedAmmoAmount)
{
	if (!bAreWeaponsLimited)
	{
		return RequestedAmmoAmount;
	}

	int32* WeaponAmmoAmount = AmmoLimits.Find(WeaponClass);
	if (WeaponAmmoAmount == nullptr)
	{
		return 0;
	}

	if (!bIsAmmoLimited)
	{
		return RequestedAmmoAmount;
	}

	int32 AvailableAmmo = *WeaponAmmoAmount < RequestedAmmoAmount ? *WeaponAmmoAmount : RequestedAmmoAmount;
	*WeaponAmmoAmount = FMath::Clamp(*WeaponAmmoAmount - RequestedAmmoAmount, 0, *WeaponAmmoAmount);

	return AvailableAmmo;
}

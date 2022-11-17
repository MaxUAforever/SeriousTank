#include "Components/ST_WeaponSocketComponent.h"

ABaseWeapon* UST_WeaponSocketComponent::SetWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	UWorld* World = GetWorld();
	if (!World || !WeaponClass)
	{
		return nullptr;
	}

	if (Weapon)
	{
		Weapon->Destroy();
	}

	Weapon = World->SpawnActor<ABaseWeapon>(WeaponClass, GetComponentTransform());
	Weapon->AttachToVehicleComponent(this);
		
	OnWeaponAdded.ExecuteIfBound(Weapon);
	
	return Weapon;
}


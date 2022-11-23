#include "Components/ST_WeaponSocketComponent.h"

ABaseWeapon* UST_WeaponSocketComponent::SetWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	UWorld* World = GetWorld();
	if (!World || !WeaponClass)
	{
		return nullptr;
	}

    DestroyWeapon();

	Weapon = World->SpawnActor<ABaseWeapon>(WeaponClass, GetComponentTransform());
	Weapon->AttachToVehicleComponent(this);
		
	OnWeaponAdded.ExecuteIfBound(Weapon);
	
	return Weapon;
}

void UST_WeaponSocketComponent::DestroyWeapon()
{
    if (Weapon)
    {
        Weapon->Destroy();
    }
}

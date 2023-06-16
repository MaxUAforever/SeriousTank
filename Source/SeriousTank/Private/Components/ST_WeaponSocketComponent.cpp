#include "Components/ST_WeaponSocketComponent.h"

AST_BaseWeapon* UST_WeaponSocketComponent::SetWeapon(TSubclassOf<AST_BaseWeapon> WeaponClass)
{
	UWorld* World = Weapon ? Weapon->GetWorld() : GetWorld();
	if (!World || !WeaponClass)
	{
		return nullptr;
	}

    DestroyWeapon();

	Weapon = World->SpawnActor<AST_BaseWeapon>(WeaponClass, GetComponentTransform());
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

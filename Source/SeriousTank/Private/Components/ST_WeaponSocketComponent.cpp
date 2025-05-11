#include "Components/ST_WeaponSocketComponent.h"

#include "Engine/World.h"

AST_BaseWeapon* UST_WeaponSocketComponent::SetWeapon(TSubclassOf<AST_BaseWeapon> WeaponClass)
{
	UWorld* World = Weapon ? Weapon->GetWorld() : GetWorld();
	if (!World || !WeaponClass)
	{
		return nullptr;
	}

    DestroyWeapon();

	Weapon = World->SpawnActor<AST_BaseWeapon>(WeaponClass, GetComponentTransform());
	Weapon->AttachToParentComponent(this);
	
	return Weapon;
}

void UST_WeaponSocketComponent::DestroyWeapon()
{
    if (Weapon)
    {
        Weapon->Destroy();
    }
}

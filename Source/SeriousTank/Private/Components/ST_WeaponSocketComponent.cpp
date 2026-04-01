#include "Components/ST_WeaponSocketComponent.h"

#include "Engine/World.h"

AST_BaseWeapon* UST_WeaponSocketComponent::SetWeapon(TSubclassOf<AST_BaseWeapon> WeaponClass)
{
	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Invalid WeaponClass provided"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	UWorld* World = Weapon ? Weapon->GetWorld() : GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ailed to get invalid world"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

    DestroyWeapon();

	Weapon = World->SpawnActor<AST_BaseWeapon>(WeaponClass, GetComponentTransform());
	if (!IsValid(Weapon))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to spawn weapon of class %s"), ANSI_TO_TCHAR(__FUNCTION__), *WeaponClass->GetName());
		return nullptr;
	}

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

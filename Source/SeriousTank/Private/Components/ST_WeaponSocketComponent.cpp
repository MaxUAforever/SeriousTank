#include "Components/ST_WeaponSocketComponent.h"

#include "Systems/GameplayAbilitySystem/Equipment/Weapons/ST_WeaponBase.h"

void UST_WeaponSocketComponent::OnChildAttached(USceneComponent* ChildComponent)
{
	AST_WeaponBase* NewWeapon = Cast<AST_WeaponBase>(ChildComponent);
	if (!NewWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_WeaponSocketComponent::OnChildAttached: Failed to cast child to weapon class."));
		return;
	}

	DestroyWeapon();
	Weapon = NewWeapon;
}

void UST_WeaponSocketComponent::DestroyWeapon()
{
    if (Weapon)
    {
        Weapon->Destroy();
    }
}

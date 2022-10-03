#include "Components/ST_WeaponsManagerComponent.h"

#include "Components/ST_WeaponSocketComponent.h"
#include "GameFramework/Actor.h"

UST_WeaponsManagerComponent::UST_WeaponsManagerComponent()
{
	CurrentWeaponIndex = 0;
}

void UST_WeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> WeaponSocketActors = GetOwner()->GetComponentsByClass(UST_WeaponSocketComponent::StaticClass());
	Weapons.Reserve(WeaponSocketActors.Num());
	for (UActorComponent* WeaponSocketActor : WeaponSocketActors)
	{
		UST_WeaponSocketComponent* WeaponSocket = Cast<UST_WeaponSocketComponent>(WeaponSocketActor);
		if (ABaseWeapon* Weapon = WeaponSocket->GetWeapon())
		{
			Weapons.Add(Weapon);
		}
	}
}

void UST_WeaponsManagerComponent::StartFire()
{
	if (Weapons.Num() != 0)
	{
		Weapons[CurrentWeaponIndex]->StartFire();
	}
}

void UST_WeaponsManagerComponent::StopFire()
{
	if (Weapons.Num() != 0)
	{
		Weapons[CurrentWeaponIndex]->StopFire();
	}
}

bool UST_WeaponsManagerComponent::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		return false;
	}

	CurrentWeaponIndex = WeaponIndex;
	return true;
}

#include "Components/ST_WeaponsManagerComponent.h"

#include "Components/ST_WeaponSocketComponent.h"
#include "GameFramework/ST_GameInstance.h"

#include "GameFramework/Actor.h"


UST_WeaponsManagerComponent::UST_WeaponsManagerComponent()
{
	CurrentWeaponIndex = 0;
}

void UST_WeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>();
	if (!GameInstance)
	{
		return;
	}

	TArray<UActorComponent*> WeaponSocketActors;
	GetOwner()->GetComponents(UST_WeaponSocketComponent::StaticClass(), WeaponSocketActors);

	Weapons.Reserve(WeaponSocketActors.Num());
	int32 WeaponIndex = 0;
	for (UActorComponent* WeaponSocketActor : WeaponSocketActors)
	{
		UST_WeaponSocketComponent* WeaponSocket = Cast<UST_WeaponSocketComponent>(WeaponSocketActor);
		TSubclassOf<ABaseWeapon> CurrentWeaponClass = GameInstance->GetWeaponClass(WeaponIndex++);
		
		ABaseWeapon* Weapon = CurrentWeaponClass ? WeaponSocket->SetWeapon(CurrentWeaponClass) : nullptr;
		Weapons.Add(Weapon);
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
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num() || Weapons[WeaponIndex] == nullptr)
	{
		return false;
	}

	StopFire();
	CurrentWeaponIndex = WeaponIndex;
	
	return true;
}

ABaseWeapon* UST_WeaponsManagerComponent::GetWeapon(int32 WeaponIndex) const
{
	return (WeaponIndex >= 0 && WeaponIndex < Weapons.Num()) ? Weapons[CurrentWeaponIndex] : nullptr;
}

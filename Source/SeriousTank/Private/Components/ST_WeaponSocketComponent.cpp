#include "Components/ST_WeaponSocketComponent.h"

void UST_WeaponSocketComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (WeaponClass)
	{
		Weapon = World->SpawnActor<ABaseWeapon>(WeaponClass, GetComponentTransform());
		Weapon->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}


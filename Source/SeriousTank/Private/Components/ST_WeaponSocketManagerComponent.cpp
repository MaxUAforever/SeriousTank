#include "Components/ST_WeaponSocketManagerComponent.h"

#include "Components/ST_WeaponSocketComponent.h"

void UST_WeaponSocketManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> WeaponSocketActors;
	GetOwner()->GetComponents(UST_WeaponSocketComponent::StaticClass(), WeaponSocketActors);
	WeaponSockets.Reserve(WeaponSocketActors.Num());

	for (UActorComponent* WeaponSocketActor : WeaponSocketActors)
	{
		WeaponSockets.Add(Cast<UST_WeaponSocketComponent>(WeaponSocketActor));
	}
}

UST_WeaponSocketComponent* UST_WeaponSocketManagerComponent::GetWeaponSocket(int32 Index) const
{
	if (!WeaponSockets.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_WeaponSocketManagerComponent::GetWeaponSocket: Invalid index."));
		return nullptr;
	}

	return WeaponSockets[Index];
}

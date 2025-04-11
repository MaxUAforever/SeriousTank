#include "Subsystems/PlayerInteractionSubsystem/Actions/ST_AmmoRefillAction.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "Components/Weapons/ST_AmmoRefillSourceComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractingComponent.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractionComponent.h"

bool UST_AmmoRefillAction::CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const
{
	if (!IsValid(InteractingComponent) || !IsValid(InteractionComponent) || !IsValid(InteractingComponent->GetOwner()) || !IsValid(InteractionComponent->GetOwner()))
	{
		return false;
	}

	if (!IsValid(InteractionComponent->GetOwner()->GetComponentByClass<UST_AmmoRefillSourceComponent>()))
	{
		return false;
	}

	if (!IsValid(InteractingComponent->GetOwner()->GetComponentByClass<UST_BaseWeaponsManagerComponent>()))
	{
		return false;
	}

	return true;
}

void UST_AmmoRefillAction::OnActivationCompleted()
{
	UInteractingComponent* InteractingComponent = GetInteractingComponent();
	UInteractionComponent* InteractionComponent = GetInteractionComponent();
	if (!CanBeActivated(InteractingComponent, InteractionComponent))
	{
		return;
	}

	UST_AmmoRefillSourceComponent* AmmoRefillSourceComponent = InteractionComponent->GetOwner()->GetComponentByClass<UST_AmmoRefillSourceComponent>();
	UST_BaseWeaponsManagerComponent* TargetWeaponsManagerComponent = InteractingComponent->GetOwner()->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
	
	for (AST_BaseWeapon* Weapon : TargetWeaponsManagerComponent->GetWeapons())
	{
		if (!IsValid(Weapon))
		{
			continue;
		}

		if (AmmoRefillSourceComponent->HasAvailableAmmo(Weapon->StaticClass()))
		{
			int32 NeededAmmo = Weapon->GetMaxAmmoCount() - Weapon->GetTotalAmmoCount();

			Weapon->AddAmmo(AmmoRefillSourceComponent->TakeAvailableAmmo(Weapon->StaticClass(), NeededAmmo));
		}
	}
}

#include "Subsystems/PlayerInteractionSubsystem/Actions/ST_WeaponPickUpAction.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractingComponent.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractionComponent.h"

bool UST_WeaponPickUpAction::CanBeActivatedAutomatically(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const
{
	if (!CanBeActivated(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	UST_BaseWeaponsManagerComponent* TargetWeaponsManagerComponent = InteractingComponent->GetOwner()->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
	if (!IsValid(TargetWeaponsManagerComponent))
	{
		return false;
	}

	// If we already have this equipped weapon - we can refill it's ammo.
	AST_BaseWeapon* EquippedWeapon = GetWeaponByClass(TargetWeaponsManagerComponent, InteractionComponent->GetOwner()->GetClass());
	if (IsValid(EquippedWeapon) && EquippedWeapon->GetTotalAmmoCount() < EquippedWeapon->GetMaxAmmoCount())
	{
		return true;
	}

	return TargetWeaponsManagerComponent->GetWeapons().Num() < TargetWeaponsManagerComponent->GetMaxWeaponsAmount();
}

bool UST_WeaponPickUpAction::CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const
{
	if (!IsValid(InteractingComponent) || !IsValid(InteractionComponent) || !IsValid(InteractingComponent->GetOwner()) || !IsValid(InteractionComponent->GetOwner()))
	{
		return false;
	}

	if (!InteractionComponent->GetOwner()->IsA(AST_BaseWeapon::StaticClass()))
	{
		return false;
	}

	if (!IsValid(InteractingComponent->GetOwner()->GetComponentByClass<UST_BaseWeaponsManagerComponent>()))
	{
		return false;
	}

	return true;
}

bool UST_WeaponPickUpAction::ActivateAutomatically(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!CanBeActivatedAutomatically(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	AST_BaseWeapon* ActionOwnerWeapon = Cast<AST_BaseWeapon>(InteractionComponent->GetOwner());
	UST_BaseWeaponsManagerComponent* TargetWeaponsManagerComponent = InteractingComponent->GetOwner()->GetComponentByClass<UST_BaseWeaponsManagerComponent>();

	AST_BaseWeapon* EquippedWeapon = GetWeaponByClass(TargetWeaponsManagerComponent, InteractionComponent->GetOwner()->GetClass());
	if (!IsValid(EquippedWeapon))
	{
		return TargetWeaponsManagerComponent->AddWeapon(ActionOwnerWeapon);
	}

	if (EquippedWeapon->GetTotalAmmoCount() < EquippedWeapon->GetMaxAmmoCount())
	{
		EquippedWeapon->AddAmmo(ActionOwnerWeapon->GetTotalAmmoCount());

		ActionOwnerWeapon->SetActorHiddenInGame(true);
		ActionOwnerWeapon->SetActorEnableCollision(false);
		ActionOwnerWeapon->Destroy();

		return true;
	}
	
	return false;
}

FText UST_WeaponPickUpAction::GetActionDescription() const
{
	return FText::FromString(TEXT("Switch Weapon"));
}

AST_BaseWeapon* UST_WeaponPickUpAction::GetWeaponByClass(UST_BaseWeaponsManagerComponent* WeaponsManagerComponent, TSubclassOf<AST_BaseWeapon> WeaponClass) const
{
	if (!IsValid(WeaponsManagerComponent) || !IsValid(WeaponClass))
	{
		return nullptr;
	}

	for (AST_BaseWeapon* Weapon : WeaponsManagerComponent->GetWeapons())
	{
		if (IsValid(Weapon) && Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	return nullptr;
}

//void UST_WeaponPickUpAction::OnActivationCompleted()
//{
//	UInteractingComponent* InteractingComponent = GetInteractingComponent();
//	UInteractionComponent* InteractionComponent = GetInteractionComponent();
//	if (!CanBeActivated(InteractingComponent, InteractionComponent))
//	{
//		return;
//	}
//
//	UST_AmmoRefillSourceComponent* AmmoRefillSourceComponent = InteractionComponent->GetOwner()->GetComponentByClass<UST_AmmoRefillSourceComponent>();
//	UST_BaseWeaponsManagerComponent* TargetWeaponsManagerComponent = InteractingComponent->GetOwner()->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
//	
//	for (AST_BaseWeapon* Weapon : TargetWeaponsManagerComponent->GetWeapons())
//	{
//		if (!IsValid(Weapon))
//		{
//			continue;
//		}
//
//		if (AmmoRefillSourceComponent->HasAvailableAmmo(Weapon->StaticClass()))
//		{
//			int32 NeededAmmo = Weapon->GetMaxAmmoCount() - Weapon->GetTotalAmmoCount();
//
//			Weapon->AddAmmo(AmmoRefillSourceComponent->TakeAvailableAmmo(Weapon->StaticClass(), NeededAmmo));
//		}
//	}
//}

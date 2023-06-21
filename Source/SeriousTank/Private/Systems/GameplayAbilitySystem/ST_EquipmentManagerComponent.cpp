#include "Systems/GameplayAbilitySystem/ST_EquipmentManagerComponent.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"
#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"

UST_EquipmentManagerComponent::UST_EquipmentManagerComponent()
{	
	SetMaxItemsAmount(3);
}

void UST_EquipmentManagerComponent::SetMaxItemsAmount(int32 InMaxItemsAmount)
{
	if (InMaxItemsAmount >= 0)
	{
		MaxItemsAmount = InMaxItemsAmount;
		EquippedItems.Reserve(MaxItemsAmount);
		EquippedItems.AddUninitialized(MaxItemsAmount);
	}
}

void UST_EquipmentManagerComponent::GrantEquipmentAbilities(AST_EquippableAbilityItem* AbilityItem)
{
	if (EquippedItems.Num() >= MaxItemsAmount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: Max amount of abilities is granted."));
		return;
	}

	GrantEquipmentAbilitiesByIndex(AbilityItem, EquippedItems.Num());
}

void UST_EquipmentManagerComponent::GrantEquipmentAbilitiesByIndex(AST_EquippableAbilityItem* AbilityItem, int32 Index)
{
	if (!AbilityItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: Failed to get AbilityItem."));
		return;
	}

	UST_VehicleAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponent();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: Failed to get AbilitySystemComponent."));
		return;
	}

	const FEquipmentAbilitySet& AbilitySet = AbilityItem->GetAbilitySet();
	if (!AbilitySet.GameplayAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: GameplayAbilityClass is not defined."));
		return;
	}

	if (!EquippedItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: Index is out of range."));
		return;
	}

	UGameplayAbility* AbilityCDO = AbilitySet.GameplayAbilityClass->GetDefaultObject<UGameplayAbility>();

	FGameplayAbilitySpec AbilitySpec(AbilityCDO);
	AbilitySpec.SourceObject = Cast<UObject>(AbilityItem);
	AbilitySpec.DynamicAbilityTags.AppendTags(AbilityCDO->AbilityTags);

	const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilityComponent->GiveAbility(AbilitySpec);

	EquippedItems[Index] = AbilityItem;
	OnItemEquipped.Broadcast(AbilityItem, Index);
}

void UST_EquipmentManagerComponent::RemoveEquipment(AST_EquippableAbilityItem* AbilityItem)
{
	for (int32 Index = 0; Index < EquippedItems.Num(); ++Index)
	{
		if (EquippedItems[Index] == AbilityItem)
		{
			RemoveEquipmentByIndex(Index);
			break;
		}
	}
}

void UST_EquipmentManagerComponent::RemoveEquipmentByIndex(int32 Index)
{
	UST_VehicleAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponent();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::RemoveEquipmentByIndex: Failed to get AbilitySystemComponent."));
		return;
	}

	if (!EquippedItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::RemoveEquipmentByIndex: Index is out of range."));
		return;
	}

	AST_EquippableAbilityItem* AbilityItem = EquippedItems[Index];
	if (AbilityItem && AbilityItem->GetAbilityHandle().IsSet())
	{
		AbilityComponent->ClearAbility(AbilityItem->GetAbilityHandle().GetValue());
	}
	
	EquippedItems[Index] = nullptr;
}

bool UST_EquipmentManagerComponent::ActivateAbility(AST_EquippableAbilityItem* AbilityItem) const
{
	for (int32 Index = 0; Index < EquippedItems.Num(); ++Index)
	{
		if (EquippedItems[Index] == AbilityItem)
		{
			return ActivateAbilityByIndex(Index);
		}
	}

	return false;
}

bool UST_EquipmentManagerComponent::ActivateAbilityByIndex(const int32 Index) const
{
	UST_VehicleAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponent();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::RemoveEquipmentByIndex: Failed to get AbilitySystemComponent."));
		return false;
	}

	if (!EquippedItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::ActivateAbilityByIndex: Index is out of range."));
		return false;
	}

	const AST_EquippableAbilityItem* AbilityItem = EquippedItems[Index];
	if (!AbilityItem || !AbilityItem->GetAbilityHandle().IsSet())
	{
		UE_LOG(LogTemp, Display, TEXT("ST_EquipmentManagerComponent::ActivateAbilityByIndex: Try to activate empty ability."));
		return false;
	}

	return AbilityComponent->TryActivateAbility(AbilityItem->GetAbilityHandle().GetValue());
}

UST_VehicleAbilitySystemComponent* UST_EquipmentManagerComponent::GetAbilitySystemComponent() const
{
	return Cast<UST_VehicleAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
}

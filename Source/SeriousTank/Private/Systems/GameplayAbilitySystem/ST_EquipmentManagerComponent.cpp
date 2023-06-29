#include "Systems/GameplayAbilitySystem/ST_EquipmentManagerComponent.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"
#include "Systems/GameplayAbilitySystem/ST_AbilitySet.h"
#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"

// TODO: Remove after test.
#include "Systems/GameplayAbilitySystem/Abilities/ST_GunFireGameplayAbility.h"

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
		EquippedItems.AddDefaulted(MaxItemsAmount);
	}
}

void UST_EquipmentManagerComponent::EquipItem(AST_EquippableAbilityItem* AbilityItem, USceneComponent* ParentComponent)
{
	for (int32 Index = 0; Index < EquippedItems.Num(); ++Index)
	{
		if (EquippedItems[Index] == nullptr)
		{
			EquipItemByIndex(AbilityItem, Index, ParentComponent);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::EquipItem: Max amount of items is equipped."));
}

void UST_EquipmentManagerComponent::EquipItemByIndex(AST_EquippableAbilityItem* AbilityItem, int32 Index, USceneComponent* ParentComponent)
{
	if (!AbilityItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::EquipItemByIndex: Failed to get AbilityItem."));
		return;
	}

	if (!EquippedItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: Index is out of range."));
		return;
	}

	AbilityItem->AttachToParentActor(GetOwner(), ParentComponent);

	bool bIsAbilityGranted = GrantEquipmentAbilitiesByIndex(AbilityItem, Index);
	if (bIsAbilityGranted)
	{
		EquippedItems[Index] = AbilityItem;
		OnItemEquipped.Broadcast(AbilityItem, Index);
	}
	
}

void UST_EquipmentManagerComponent::EquipItemByIndex(TSubclassOf<AST_EquippableAbilityItem> AbilityItemClass, int32 Index, USceneComponent* ParentComponent)
{
	if (!AbilityItemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::EquipItemByIndex: AbilityItemClass is not defined."))
		return;
	}

	AST_EquippableAbilityItem* AbilityItem;
	
	UWorld* World = GetWorld();
	if (World && ParentComponent)
	{
		AbilityItem = World->SpawnActor<AST_EquippableAbilityItem>(AbilityItemClass, ParentComponent->GetComponentTransform());
	}
	else
	{
		AbilityItem = NewObject<AST_EquippableAbilityItem>(this);
	}

	EquipItemByIndex(AbilityItem, Index, ParentComponent);
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

bool UST_EquipmentManagerComponent::GrantEquipmentAbilitiesByIndex(AST_EquippableAbilityItem* AbilityItem, int32 Index)
{
	UST_VehicleAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponent();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: Failed to get AbilitySystemComponent."));
		return false;
	}

	const FST_AbilitySet& AbilitySet = AbilityItem->GetAbilitySet();
	if (!AbilitySet.GameplayAbilityInfo.GameplayAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_EquipmentManagerComponent::GrantEquipmentAbilities: GameplayAbilityClass is not defined."));
		return false;
	}

	EST_AbilityInputID InputID = static_cast<EST_AbilityInputID>(static_cast<int32>(EST_AbilityInputID::EquippedItem1) + Index);
	FGameplayAbilitySpecHandle AbilitySpecHandle = AbilityComponent->GiveAbility(AbilitySet.GameplayAbilityInfo, Cast<UObject>(AbilityItem), InputID);
	
	return AbilitySpecHandle != FGameplayAbilitySpecHandle{};
}

UST_VehicleAbilitySystemComponent* UST_EquipmentManagerComponent::GetAbilitySystemComponent() const
{
	return Cast<UST_VehicleAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
}

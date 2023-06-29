#include "Systems/GameplayAbilitySystem/Abilities/ST_EquippedItemAbility.h"

#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"

bool UST_EquippedItemAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AST_EquippableAbilityItem* AbilityItem = GetAbilityItem(Handle, ActorInfo))
	{
		return AbilityItem->CheckCost();
	}

	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UST_EquippedItemAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (AST_EquippableAbilityItem* AbilityItem = GetAbilityItem(Handle, ActorInfo))
	{
		AbilityItem->ApplyCost();
		return;
	}

	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

void UST_EquippedItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	OnItemAbilityActivated.Broadcast(Handle);
}

AST_EquippableAbilityItem* UST_EquippedItemAbility::GetAbilityItem(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
	return Cast<AST_EquippableAbilityItem>(GetSourceObject(Handle, ActorInfo));
}

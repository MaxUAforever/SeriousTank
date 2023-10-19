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
	if (AST_EquippableAbilityItem* AbilityItem = GetAbilityItem(Handle, ActorInfo))
	{
		AbilityItem->HandleAbilityActivated();
		return;
	}
}

void UST_EquippedItemAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AST_EquippableAbilityItem* AbilityItem = GetAbilityItem(Handle, ActorInfo))
	{
		AbilityItem->HandleAbilityEnded();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AST_EquippableAbilityItem* UST_EquippedItemAbility::GetAbilityItem(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
	return Cast<AST_EquippableAbilityItem>(GetSourceObject(Handle, ActorInfo));
}

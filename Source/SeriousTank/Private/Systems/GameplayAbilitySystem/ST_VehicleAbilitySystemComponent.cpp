#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"
#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"
#include "Systems/GameplayAbilitySystem/ST_AbilitySet.h"

FGameplayAbilitySpecHandle UST_VehicleAbilitySystemComponent::GiveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	return Super::GiveAbility(AbilitySpec);
}

FGameplayAbilitySpecHandle UST_VehicleAbilitySystemComponent::GiveAbility(const FST_GameplayAbilityInfo& AbilityInfo, UObject* SourceObject, EST_AbilityInputID InputID)
{
	if (!AbilityInfo.GameplayAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_VehicleAbilitySystemComponent::GiveAbility: GameplayAbilityClass is not defined."));
		return FGameplayAbilitySpecHandle();
	}

	UGameplayAbility* AbilityCDO = AbilityInfo.GameplayAbilityClass->GetDefaultObject<UGameplayAbility>();

	FGameplayAbilitySpec AbilitySpec(AbilityCDO);
	AbilitySpec.SourceObject = SourceObject;
	AbilitySpec.DynamicAbilityTags.AppendTags(AbilityCDO->AbilityTags);
	AbilitySpec.InputID = static_cast<int32>(InputID);

	return GiveAbility(AbilitySpec);
}

void UST_VehicleAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (AST_EquippableAbilityItem* AbilityItem = Cast<AST_EquippableAbilityItem>(AbilitySpec.SourceObject))
	{
		AbilityItem->OnGiveAbility(AbilitySpec);
	}
}

void UST_VehicleAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);

	if (AST_EquippableAbilityItem* AbilityItem = Cast<AST_EquippableAbilityItem>(AbilitySpec.SourceObject))
	{
		AbilityItem->OnRemoveAbility(AbilitySpec);
	}
}
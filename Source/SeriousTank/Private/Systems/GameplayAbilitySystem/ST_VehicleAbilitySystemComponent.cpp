#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"

#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"

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
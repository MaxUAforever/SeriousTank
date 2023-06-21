#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"

#include "Abilities/GameplayAbility.h"

void AST_EquippableAbilityItem::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	AbilitySpecHandle = MakeUnique<FGameplayAbilitySpecHandle>(AbilitySpec.Handle);
}

void AST_EquippableAbilityItem::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	AbilitySpecHandle.Reset();
}

TOptional<FGameplayAbilitySpecHandle> AST_EquippableAbilityItem::GetAbilityHandle() const
{
	return AbilitySpecHandle ? TOptional<FGameplayAbilitySpecHandle>(*AbilitySpecHandle.Get()) : TOptional<FGameplayAbilitySpecHandle>();
}

#pragma once

#include "Abilities/GameplayAbility.h"
#include "ST_EquippedItemAbility.generated.h"

class AST_EquippableAbilityItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAbilityActivated, const FGameplayAbilitySpecHandle);

UCLASS()
class SERIOUSTANK_API UST_EquippedItemAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	FOnItemAbilityActivated OnItemAbilityActivated;

public:
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	AST_EquippableAbilityItem* GetAbilityItem(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const;
};

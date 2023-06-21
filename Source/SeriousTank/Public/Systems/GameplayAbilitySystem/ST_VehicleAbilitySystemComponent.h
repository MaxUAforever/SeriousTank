#pragma once

#include "AbilitySystemComponent.h"
#include "ST_VehicleAbilitySystemComponent.generated.h"

UCLASS()
class SERIOUSTANK_API UST_VehicleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};

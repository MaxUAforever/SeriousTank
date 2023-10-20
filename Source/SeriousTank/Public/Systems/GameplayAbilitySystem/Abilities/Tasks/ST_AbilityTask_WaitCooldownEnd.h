#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "ST_AbilityTask_WaitCooldownEnd.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownEndedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmReleasedDelegate);

/**
 * UST_AbilityTask_WaitCooldownEnd can be used to wait until cooldown effect ends or confirm button is released before it.
 */
UCLASS()
class SERIOUSTANK_API UST_AbilityTask_WaitCooldownEnd : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownEndedDelegate OnCooldownEndedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmReleasedDelegate OnConfirmReleasedDelegate;

private:
	UGameplayAbility* OwningAbility;
	FActiveGameplayEffectHandle Handle;

	FDelegateHandle OnGameplayEffectRemovedDelegateHandle;

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UST_AbilityTask_WaitCooldownEnd* WaitCooldownEnd(UGameplayAbility* InOwningAbility, FActiveGameplayEffectHandle InHandle);

protected:
	UFUNCTION()
	void OnCooldownEffectRemoved(const FGameplayEffectRemovalInfo& InGameplayEffectRemovalInfo);

	UFUNCTION()
	void OnConfirmButtonReleased();
};

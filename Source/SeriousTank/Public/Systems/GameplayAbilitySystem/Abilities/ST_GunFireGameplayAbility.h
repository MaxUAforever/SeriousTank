#pragma once

#include "Systems/GameplayAbilitySystem/Abilities/ST_EquippedItemAbility.h"
#include "ST_GunFireGameplayAbility.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCooldownEffectAppliedDelegate, FActiveGameplayEffectHandle);

UCLASS()
class SERIOUSTANK_API UST_GunFireGameplayAbility : public UST_EquippedItemAbility
{
	GENERATED_BODY()

public:
	FOnCooldownEffectAppliedDelegate OnCooldownEffectAppliedDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

private:
	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	FActiveGameplayEffectHandle CooldownEffectHandle;

public:
	UST_GunFireGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	const FGameplayTagContainer* GetCooldownTags() const override;

	void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

private:
	UFUNCTION()
	void OnConfirm();

	UFUNCTION()
	void OnCancel();

	UFUNCTION()
	void OnAbilityChanged(UGameplayAbility* ActivatedAbility);

	UFUNCTION()
	void OnConfirmTaskActivated();

	UFUNCTION()
	void RestartConfirmTask();

	void OnCooldownEffectApplied(FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};

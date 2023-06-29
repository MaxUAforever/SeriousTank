#pragma once

#include "Systems/GameplayAbilitySystem/Abilities/ST_EquippedItemAbility.h"
#include "ST_GunFireGameplayAbility.generated.h"

UCLASS()
class SERIOUSTANK_API UST_GunFireGameplayAbility : public UST_EquippedItemAbility
{
	GENERATED_BODY()

private:
	bool bIsWaiting = false;

public:
	UST_GunFireGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnConfirm();

	UFUNCTION()
	void OnCancel();

	UFUNCTION()
	void OnAbilityChanged(UGameplayAbility* ActivatedAbility);

	UFUNCTION()
	void OnProjectileSpawned(AActor* SpawnedProjectile);
};

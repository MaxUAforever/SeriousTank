#include "Systems/GameplayAbilitySystem/Abilities/ST_GunFireGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitAbilityActivate.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirmCancel.h"
#include "Systems/GameplayAbilitySystem/Abilities/Tasks/ST_AbilityTask_SpawnProjectile.h"

UST_GunFireGameplayAbility::UST_GunFireGameplayAbility()
{
	FGameplayTag TargetingTag = FGameplayTag::RequestGameplayTag(FName("Ability.Targeting"));
	AbilityTags.AddTag(TargetingTag);
	ActivationOwnedTags.AddTag(TargetingTag);
}

void UST_GunFireGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitConfirmCancel* WaitConfirmTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this);
	WaitConfirmTask->OnConfirm.AddDynamic(this, &ThisClass::OnConfirm);
	WaitConfirmTask->OnCancel.AddDynamic(this, &ThisClass::OnCancel);
	WaitConfirmTask->ReadyForActivation();

	UAbilityTask_WaitAbilityActivate* WaitAbilityActivate = UAbilityTask_WaitAbilityActivate::WaitForAbilityActivate(this, FGameplayTag::RequestGameplayTag(FName("Ability.Targeting")), FGameplayTag::EmptyTag);
	WaitAbilityActivate->OnActivate.AddDynamic(this, &ThisClass::OnAbilityChanged);
	WaitAbilityActivate->ReadyForActivation();
}

void UST_GunFireGameplayAbility::OnConfirm()
{
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UST_AbilityTask_SpawnProjectile* SpawnProjectileTask = UST_AbilityTask_SpawnProjectile::SpawnProjectile(this);
	SpawnProjectileTask->OnSpawnProjectile.AddDynamic(this, &ThisClass::OnProjectileSpawned);
	SpawnProjectileTask->ReadyForActivation();
}

void UST_GunFireGameplayAbility::OnCancel()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UST_GunFireGameplayAbility::OnAbilityChanged(UGameplayAbility* ActivatedAbility)
{
	OnCancel();
}

void UST_GunFireGameplayAbility::OnProjectileSpawned(AActor* SpawnedProjectile)
{
	UAbilityTask_WaitConfirmCancel* WaitConfirmTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this);
	WaitConfirmTask->OnConfirm.AddDynamic(this, &ThisClass::OnConfirm);
	WaitConfirmTask->OnCancel.AddDynamic(this, &ThisClass::OnCancel);
	WaitConfirmTask->ReadyForActivation();
}

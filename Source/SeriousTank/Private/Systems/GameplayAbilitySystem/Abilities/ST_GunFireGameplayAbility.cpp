#include "Systems/GameplayAbilitySystem/Abilities/ST_GunFireGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitAbilityActivate.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirmCancel.h"
#include "Systems/GameplayAbilitySystem/Abilities/Tasks/ST_AbilityTask_SpawnProjectile.h"
#include "Systems/GameplayAbilitySystem/Abilities/Tasks/ST_AbilityTask_WaitCooldownEnd.h"


UST_GunFireGameplayAbility::UST_GunFireGameplayAbility()
{
	FGameplayTag TargetingTag = FGameplayTag::RequestGameplayTag(FName("Ability.Targeting"));
	AbilityTags.AddTag(TargetingTag);
	ActivationOwnedTags.AddTag(TargetingTag);
}

void UST_GunFireGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OnCooldownEffectAppliedDelegate.AddUObject(this, &ThisClass::OnCooldownEffectApplied);

	UAbilityTask_WaitConfirmCancel* WaitConfirmTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this);
	WaitConfirmTask->OnConfirm.AddDynamic(this, &ThisClass::OnConfirm);
	WaitConfirmTask->OnCancel.AddDynamic(this, &ThisClass::OnCancel);
	WaitConfirmTask->ReadyForActivation();

	UAbilityTask_WaitAbilityActivate* WaitAbilityActivate = UAbilityTask_WaitAbilityActivate::WaitForAbilityActivate(this, FGameplayTag::RequestGameplayTag(FName("Ability.Targeting")), FGameplayTag::EmptyTag);
	WaitAbilityActivate->OnActivate.AddDynamic(this, &ThisClass::OnAbilityChanged);
	WaitAbilityActivate->ReadyForActivation();
}

const FGameplayTagContainer* UST_GunFireGameplayAbility::GetCooldownTags() const
{
	// MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset();
	
	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		MutableTags->AppendTags(*ParentTags);
	}

	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UST_GunFireGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Cooldown")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		FActiveGameplayEffectHandle NewCooldownEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);

		OnCooldownEffectAppliedDelegate.Broadcast(NewCooldownEffectHandle);
	}
}

void UST_GunFireGameplayAbility::OnConfirm()
{
	UST_AbilityTask_WaitCooldownEnd* WaitCooldownEndTask = UST_AbilityTask_WaitCooldownEnd::WaitCooldownEnd(this, CooldownEffectHandle);
	WaitCooldownEndTask->OnCooldownEndedDelegate.AddDynamic(this, &ThisClass::OnConfirmTaskActivated);
	WaitCooldownEndTask->OnConfirmReleasedDelegate.AddDynamic(this, &ThisClass::RestartConfirmTask);
	WaitCooldownEndTask->ReadyForActivation();
}

void UST_GunFireGameplayAbility::OnCancel()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UST_GunFireGameplayAbility::OnAbilityChanged(UGameplayAbility* ActivatedAbility)
{
	OnCancel();
}

void UST_GunFireGameplayAbility::OnConfirmTaskActivated()
{
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		UST_AbilityTask_SpawnProjectile* SpawnProjectileTask = UST_AbilityTask_SpawnProjectile::SpawnProjectile(this);
		SpawnProjectileTask->ReadyForActivation();
	}

	OnConfirm();
}

void UST_GunFireGameplayAbility::RestartConfirmTask()
{
	UAbilityTask_WaitConfirmCancel* WaitConfirmTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this);
	WaitConfirmTask->OnConfirm.AddDynamic(this, &ThisClass::OnConfirm);
	WaitConfirmTask->OnCancel.AddDynamic(this, &ThisClass::OnCancel);
	WaitConfirmTask->ReadyForActivation();
}

void UST_GunFireGameplayAbility::OnCooldownEffectApplied(FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	CooldownEffectHandle = ActiveGameplayEffectHandle;
}
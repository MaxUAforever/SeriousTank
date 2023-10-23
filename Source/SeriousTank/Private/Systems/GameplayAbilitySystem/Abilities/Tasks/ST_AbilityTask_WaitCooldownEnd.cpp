#include "Systems/GameplayAbilitySystem/Abilities/Tasks/ST_AbilityTask_WaitCooldownEnd.h"

#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ST_AbilityTask_WaitCooldownEnd)

void UST_AbilityTask_WaitCooldownEnd::Activate()
{
	UST_VehicleAbilitySystemComponent* VehicleAbilitySystemCompoent = Cast<UST_VehicleAbilitySystemComponent>(AbilitySystemComponent);
	if (!VehicleAbilitySystemCompoent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityTask_RepeatTask::Activate: Failed to get AbilitySystemComponent."));
		return;
	}

	if (FOnActiveGameplayEffectRemoved_Info* DelPtr = VehicleAbilitySystemCompoent->OnGameplayEffectRemoved_InfoDelegate(Handle))
	{
		OnGameplayEffectRemovedDelegateHandle = DelPtr->AddUObject(this, &ThisClass::OnCooldownEffectRemoved);

		VehicleAbilitySystemCompoent->OnConfirmInputReleased.AddUObject(this, &ThisClass::OnConfirmButtonReleased);
	}
	else
	{
		// If effect isn't found by given handle - notify immideately.
		OnCooldownEndedDelegate.Broadcast();
		EndTask();
	}
}

UST_AbilityTask_WaitCooldownEnd* UST_AbilityTask_WaitCooldownEnd::WaitCooldownEnd(UGameplayAbility* InOwningAbility, FActiveGameplayEffectHandle InHandle)
{
	UST_AbilityTask_WaitCooldownEnd* TaskObject = NewAbilityTask<UST_AbilityTask_WaitCooldownEnd>(InOwningAbility);
	TaskObject->OwningAbility = InOwningAbility;
	TaskObject->Handle = InHandle;

	return TaskObject;
}

void UST_AbilityTask_WaitCooldownEnd::OnCooldownEffectRemoved(const FGameplayEffectRemovalInfo& InGameplayEffectRemovalInfo)
{
	OnCooldownEndedDelegate.Broadcast();
	EndTask();
}

void UST_AbilityTask_WaitCooldownEnd::OnConfirmButtonReleased()
{
	OnGameplayEffectRemovedDelegateHandle.Reset();

	OnConfirmReleasedDelegate.Broadcast();
	EndTask();
}
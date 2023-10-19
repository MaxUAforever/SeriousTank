#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"

#include "AbilitySystemGlobals.h"
#include "Systems/GameplayAbilitySystem/Abilities/ST_EquippedItemAbility.h"
#include "Systems/GameplayAbilitySystem/ST_AbilitySet.h"
#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"

void AST_EquippableAbilityItem::AttachToParentActor(AActor* ParentActor, USceneComponent* ParentActorComponent)
{
	if (ParentActor)
	{
		ParentActor->OnDestroyed.AddDynamic(this, &ThisClass::OnParentDestroyed);
		SetOwner(ParentActor);
	}

	if (ParentActorComponent)
	{
		AttachToComponent(ParentActorComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AST_EquippableAbilityItem::OnGiveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	AbilitySpecHandle = MakeUnique<FGameplayAbilitySpecHandle>(AbilitySpec.Handle);
}

void AST_EquippableAbilityItem::OnRemoveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	AbilitySpecHandle.Reset();
}

TOptional<FGameplayAbilitySpecHandle> AST_EquippableAbilityItem::GetAbilityHandle() const
{
	return AbilitySpecHandle ? TOptional<FGameplayAbilitySpecHandle>(*AbilitySpecHandle.Get()) : TOptional<FGameplayAbilitySpecHandle>();
}

void AST_EquippableAbilityItem::OnParentDestroyed(AActor* DestroyedOwnerActor)
{
	Destroy();
}

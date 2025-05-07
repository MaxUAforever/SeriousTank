#include "Subsystems/AIManagerSubsystem/ST_AIManagerSubsystem.h"


#include "PlayerInteractionSubsystem/Public/Components/InteractionComponent.h"
#include "PlayerInteractionSubsystem/Public/PlayerInteractionSubsystem.h"
#include "Subsystems/PlayerInteractionSubsystem/Actions/ST_AmmoRefillAction.h"
#include "Subsystems/PlayerInteractionSubsystem/Actions/ST_WeaponPickUpAction.h"
#include "Engine/World.h"

void UST_AIManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	InitInteractionItems(InWorld);
}

const TSet<TObjectPtr<const AActor>>* UST_AIManagerSubsystem::GetInteractionActors(EAIInteractionType InteractionType) const
{
	const FInteractionTypeInfo* InteractionTypeInfo = InteractionsInfo.Find(InteractionType);
	if (InteractionTypeInfo != nullptr)
	{
		return &InteractionTypeInfo->InteractionActors;
	}

	return nullptr;
}

void UST_AIManagerSubsystem::InitInteractionItems(const UWorld& InWorld)
{
	UPlayerInteractionSubsystem* InteractionSubsystem = InWorld.GetSubsystem<UPlayerInteractionSubsystem>();
	if (!IsValid(InteractionSubsystem))
	{
		return;
	}

	InteractionSubsystem->GetOnIsInteractionComponentActiveChangedDelegate().AddUObject(this, &ThisClass::OnIsInteractionComponentActiveChanged);

	for (EAIInteractionType InteractionType : TEnumRange<EAIInteractionType>())
	{
		InteractionsInfo.Add(InteractionType, {});
	}

	for (const UInteractionComponent* InteractionComponent : InteractionSubsystem->GetActiveInteractionComponents())
	{
		AddInteractionComponent(InteractionComponent);
	}
}

void UST_AIManagerSubsystem::AddInteractionComponent(const UInteractionComponent* InteractionComponent)
{
	if (!IsValid(InteractionComponent))
	{
		return;
	}

	FInteractionTypeInfo* InteractionTypeInfo = InteractionsInfo.Find(GetInteractionTypeByClass(InteractionComponent->GetActionClass()));
	if (InteractionTypeInfo != nullptr && InteractionComponent->IsInteractionComponentActive())
	{
		InteractionTypeInfo->InteractionActors.Add(InteractionComponent->GetOwner());
	}
}

void UST_AIManagerSubsystem::RemoveInteractionComponent(const UInteractionComponent* InteractionComponent)
{
	if (!IsValid(InteractionComponent))
	{
		return;
	}

	FInteractionTypeInfo* InteractionTypeInfo = InteractionsInfo.Find(GetInteractionTypeByClass(InteractionComponent->GetActionClass()));
	if (InteractionTypeInfo != nullptr)
	{
		InteractionTypeInfo->InteractionActors.Remove(InteractionComponent->GetOwner());
	}
}

void UST_AIManagerSubsystem::OnIsInteractionComponentActiveChanged(const UInteractionComponent* InteractionComponent, bool bIsActive)
{
	if (!IsValid(InteractionComponent))
	{
		return;
	}

	if (bIsActive)
	{
		AddInteractionComponent(InteractionComponent);
	}
	else
	{
		RemoveInteractionComponent(InteractionComponent);
	}
}

EAIInteractionType UST_AIManagerSubsystem::GetInteractionTypeByClass(TSubclassOf<UBaseInteractionAction> ActionClass)
{
	if (ActionClass->IsChildOf(UST_WeaponPickUpAction::StaticClass()))
	{
		return EAIInteractionType::PickingUpWeapon;
	}
	else if (ActionClass->IsChildOf(UST_AmmoRefillAction::StaticClass()))
	{
		return EAIInteractionType::RefillingAmmo;
	}
	else
	{
		return EAIInteractionType::TakingVehicle;
	}
}

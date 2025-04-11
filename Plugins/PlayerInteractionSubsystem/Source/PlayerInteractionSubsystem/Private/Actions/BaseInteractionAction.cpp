#include "Actions/BaseInteractionAction.h"

#include "Data/Actions/BaseInteractionActionDataAsset.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"

void UBaseInteractionAction::Initialize(const UBaseInteractionActionDataAsset* InteractionActionDataAsset)
{
	if (!IsValid(InteractionActionDataAsset))
	{
		return;
	}

	ActionDescription = InteractionActionDataAsset->ActionDescription;
	ActivationMontage = InteractionActionDataAsset->ActivationMontage;
	DeactivationMontage = InteractionActionDataAsset->DeactivationMontage;
	bShouldBeDeactivated = InteractionActionDataAsset->bShouldBeDeactivated;
	bIsBlockingInteraction = InteractionActionDataAsset->bIsBlockingInteraction;
}

bool UBaseInteractionAction::Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!CanBeActivated(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	return SetCachedComponents(InteractingComponent, InteractionComponent);
}

bool UBaseInteractionAction::Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!SetCachedComponents(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	return bShouldBeDeactivated;
}

bool UBaseInteractionAction::SetCachedComponents(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!IsValid(InteractingComponent) || !IsValid(InteractionComponent))
	{
		return false;
	}

	CachedInteractingComponent = InteractingComponent;
	CachedInteractionComponent = InteractionComponent;

	return true;
}
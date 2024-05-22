#include "PlayerInteractionSubsystem.h"

#include "InteractionComponent.h"

void UPlayerInteractionSubsystem::RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	InteractionMap.Add(InteractingComponent, InteractionComponent);
}

void UPlayerInteractionSubsystem::RemoveInteraction(UInteractingComponent* InteractingComponent)
{
	InteractionMap.Remove(InteractingComponent);
}

void UPlayerInteractionSubsystem::StartInteractionAction(UInteractingComponent* InteractingComponent)
{
	UInteractionComponent** InteractionComponent = InteractionMap.Find(InteractingComponent);
	if (!InteractionComponent || !(*InteractionComponent))
	{
		return;
	}

	(*InteractionComponent)->ActivateAction(InteractingComponent);
}
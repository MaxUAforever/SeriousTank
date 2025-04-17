#pragma once

#include "Actions/BaseInteractionAction.h"
#include "BaseAutomaticAction.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UBaseAutomaticAction : public UBaseInteractionAction
{
	GENERATED_BODY()

public:
	virtual bool CanBeActivatedAutomatically(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const { return true; };

	virtual bool ActivateAutomatically(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) { return true; };
};

#pragma once

#include "UObject/NoExportTypes.h"
#include "BaseInteractionAction.generated.h"

class UInteractingComponent;
class UInteractionComponent;

/**
 * Base class for action that will be performed due to interaction.
 */
UCLASS(Abstract)
class PLAYERINTERACTIONSUBSYSTEM_API UBaseInteractionAction : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) {};

	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const { return true; };

	virtual FText GetActionDescription() const { return FText{}; };
};

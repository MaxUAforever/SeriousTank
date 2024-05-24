#pragma once

#include "Actions/BaseInteractionAction.h"
#include "PossessVehicleAction.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * Action for possessing a vehicle actor if pawn with needed interactiong component called it.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UPossessVehicleAction : public UBaseInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;

	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const override;

	virtual FText GetActionDescription() const override;
};

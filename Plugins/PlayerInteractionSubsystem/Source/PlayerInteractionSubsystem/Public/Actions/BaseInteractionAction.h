#pragma once

#include "UObject/Object.h"
#include "BaseInteractionAction.generated.h"

class UAnimMontage;
class UInteractingComponent;
class UInteractionComponent;

/**
 * Base class for action that will be performed due to interaction.
 */
UCLASS(Abstract)
class PLAYERINTERACTIONSUBSYSTEM_API UBaseInteractionAction : public UObject
{
	GENERATED_BODY()

protected:
	// Action montage that can be played during performing of action.
	UPROPERTY()
	UAnimMontage* ActionMontage;

	UPROPERTY()
	UAnimMontage* DeactivationMontage;

	// Determines if action can be deactivated after interaction action.
	UPROPERTY()
	bool bShouldBeDeactivated = false;

public:
	void SetActionMontage(UAnimMontage* InActionMontage) { ActionMontage = InActionMontage; };
	void SetDeactivationMontage(UAnimMontage* InDeactivationMontage) { DeactivationMontage = InDeactivationMontage; };

	bool ShouldBeDeactivated() const { return bShouldBeDeactivated; }

	virtual bool Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) { return true; };
	virtual bool Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) { return bShouldBeDeactivated; };

	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const { return true; };
	
	virtual FText GetActionDescription() const { return FText{}; };
};

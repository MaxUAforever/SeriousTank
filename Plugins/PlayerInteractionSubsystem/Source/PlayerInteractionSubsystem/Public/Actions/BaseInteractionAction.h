#pragma once

#include "UObject/Object.h"
#include "BaseInteractionAction.generated.h"

class UAnimMontage;
class UBaseInteractionActionDataAsset;
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
	/**
	 * Initializes the interaction action with data from the specified asset.
	 * 
	 * @param InteractionActionDataAsset - Data asset containing action-specific configurations.
	 * @note To add new custom paramteres to child class - override
	 */
	virtual void Initialize(const UBaseInteractionActionDataAsset* InteractionActionDataAsset);

	/**
	 * Activates the interaction action.
	 * 
	 * @param InteractingComponent - Component representing the interacting entity.
	 * @param InteractionComponent - Component representing the object being interacted with.
	 * @return True if activation is successful, otherwise false.
	 */
	virtual bool Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);
	
	/**
	 * Deactivates active interaction action if it's continuous and currently active.
	 * 
	 * @param InteractingComponent - Component representing the interacting entity.
	 * @param InteractionComponent - Component representing the object being interacted with.
	 * @return True if deactivation is successful, otherwise false.
	 */
	virtual bool Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);

	/**
	 * Retrieves the description of the action, used in interaction widget.
	 * 
	 * @return A localized text describing the action.
	 */
	virtual FText GetActionDescription() const { return ActionDescription; };

	/**
	 * Checks if action is continuous and should be deactivated.
	 *
	 * @return True if action can be deactivated.
	 */
	bool ShouldBeDeactivated() const { return bShouldBeDeactivated; }
	void SetShouldBeDeactivated(bool bInShouldBeDeactivated) { bShouldBeDeactivated = bInShouldBeDeactivated; }

	/**
	 * Checks if action should block other user interaction while active.
	 *
	 * @return True if action is blocking.
	 */
	bool IsBlockingInteraction() const { return bIsBlockingInteraction; }
	void SetIsBlockingInteraction(bool bInIsBlockingInteraction) { bIsBlockingInteraction = bInIsBlockingInteraction; }

protected:
	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const { return true; };

	UAnimMontage* GetActivationMontage() const { return ActivationMontage; };
	UAnimMontage* GetDeactivationMontage() const { return DeactivationMontage; };

	FText GetDefaultActionDescription() const { return ActionDescription; }

	UInteractingComponent* GetInteractingComponent() const { return CachedInteractingComponent; }
	UInteractionComponent* GetInteractionComponent() const { return CachedInteractionComponent; }

private:
	bool SetCachedComponents(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);

private:
	// Action montages that can be played during performing of action.
	UPROPERTY()
	TObjectPtr<UAnimMontage> ActivationMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> DeactivationMontage;

	// Determines if action can be deactivated after interaction action.
	bool bShouldBeDeactivated = true;

	FText ActionDescription;

	bool bIsBlockingInteraction = false;

private:
	TObjectPtr<UInteractingComponent> CachedInteractingComponent;
	TObjectPtr<UInteractionComponent> CachedInteractionComponent;
};

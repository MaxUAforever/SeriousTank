#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "PlayerInteractionSubsystem.generated.h"

class UInteractingComponent;
class UInteractionComponent;

/**
 * Struct that stores information about status of InteractionComponent.
 */
struct InteractionComponentInfo
{
	UInteractionComponent* InteractionComponent;
};

/**
 * Subsystem that provides binding and checks for all interactions between
 * Interaction and Interacting components.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UPlayerInteractionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TMap<UInteractingComponent*, InteractionComponentInfo> InteractionMap;

	TMap<UInteractingComponent*, UInteractionComponent*> ActiveInteractionsMap;

	FTimerHandle InteractionsUpdateTimerHandle;
	float RefreshRate = 0.5f;

public:
	void RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);
	void RemoveInteraction(UInteractingComponent* InteractingComponent);
	void RemoveInteraction(UInteractionComponent* InteractingComponent);
	
	/**
	* Allow to start interaction action with interaction component for interacting component, 
	* if it's already registered in this subsystem.
	*
	* @param InteractingComponent - pointer to component that request interaction.
	* @return true if action is continuous and can be stopped after activation.
	*/ 
	bool StartInteractionAction(UInteractingComponent* InteractingComponent);

	/**
	* Allow to stop interaction action with interaction component,  if continuous action 
	* was started earlier for this component.
	*
	* @param InteractingComponent - pointer to component that request interaction.
	* @return true if action was stopped successfully.
	*/
	bool StopInteractionAction(UInteractingComponent* InteractingComponent);
};

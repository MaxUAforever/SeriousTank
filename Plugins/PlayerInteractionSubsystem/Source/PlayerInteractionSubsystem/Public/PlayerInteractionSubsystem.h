#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "PlayerInteractionSubsystem.generated.h"

class UInteractingComponent;
class UInteractionComponent;

/**
 * Subsystem that provides binding and checks for all interactions between
 * Interaction and Interacting components.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UPlayerInteractionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TMap<UInteractingComponent*, UInteractionComponent*> InteractionMap;

public:
	void RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);
	void RemoveInteraction(UInteractingComponent* InteractingComponent);

	void StartInteractionAction(UInteractingComponent* InteractingComponent);
};

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

	bool bCanBeActivated;
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

	FTimerHandle InteractionsUpdateTimerHandle;
	float RefreshRate = 0.5f;

public:
	void RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);
	void RemoveInteraction(UInteractingComponent* InteractingComponent);
	void RemoveInteraction(UInteractionComponent* InteractingComponent);

	void StartInteractionAction(UInteractingComponent* InteractingComponent);

private:
	void UpdateInteractionStatuses();
};

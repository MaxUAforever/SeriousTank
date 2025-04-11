#pragma once

#include "Components/ActorComponent.h"
#include "InteractingComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UInteractionComponent;
class UPlayerInteractionSubsystem;
class UWidgetComponent;

DECLARE_MULTICAST_DELEGATE(FOnInteractionActionBoundDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeactivatingInteractionActionStartedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnInteractingActionStoppedDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionRegisterStateChangedDelegate, bool, const UInteractionComponent*);

/*
* Component that should be added to pawns that should initiate interaction process.
* If an InteractionComponent is currently bounded to this component in InteractionSubsystem, 
* then action from it would be performed after calling of Interact function.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLAYERINTERACTIONSUBSYSTEM_API UInteractingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Delegate that should fire when an opportunity to interract appeared or disappeared 
	// for the current component.
	FOnInteractionRegisterStateChangedDelegate OnInteractionRegisterStateChangedDelegate;

	// Delegate that should fire when action is loaded from settings.
	FOnInteractionActionBoundDelegate OnInteractionActionBoundDelegate;

	FOnDeactivatingInteractionActionStartedDelegate OnDeactivatingActionStartedDelegate;
	FOnInteractingActionStoppedDelegate OnInteractingActionStoppedDelegate;

public:
	bool StartInteraction();
	bool StopInteraction();

	bool IsInteracting() const;
	bool IsInteractingBlockingly() const;

	bool IsInteractionEnabled() const { return bIsInteractionEnabled; }
	void SetInteractionEnabled(bool bInIsInteractionEnabled);

protected:
	virtual void BeginPlay() override;	

private:
	TObjectPtr<UPlayerInteractionSubsystem> PlayerInteractionSubsystem;
	uint32 InteractBindingHandle;

	bool bIsInteractionEnabled = true;
};

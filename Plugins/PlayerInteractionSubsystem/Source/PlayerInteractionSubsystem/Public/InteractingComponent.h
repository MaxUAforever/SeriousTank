#pragma once

#include "Components/ActorComponent.h"
#include "InteractingComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UPlayerInteractionSubsystem;
class UWidgetComponent;

DECLARE_MULTICAST_DELEGATE(FOnInteractionActionBoundDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractingStateChangedDelegate, bool);

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
	// Delegate that should fire when an opportynity to interract appeared or disappeared 
	// for the current component.
	FOnInteractingStateChangedDelegate OnInteractingStateChanged;

	// Delegate that should fire when action is bound to EnhancedComponent.
	FOnInteractionActionBoundDelegate OnInteractionActionBoundDelegate;

protected:
	/**
	 * Interaction inputs
	 */

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputMappingContext* InteractionInputContext;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* InteractInputAction;

private:
	UPlayerInteractionSubsystem* PlayerInteractionSubsystem;

public:
	const UInputAction* GetInputAction() const { return InteractInputAction; };

protected:
	virtual void BeginPlay() override;	
	
	UFUNCTION()
	void Interact();

	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);
};

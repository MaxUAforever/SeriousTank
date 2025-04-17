#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "PlayerInteractionSubsystem.generated.h"

class UBaseInteractionAction;
class UInteractingComponent;
class UInteractionComponent;
class UInteractionSubsystemSettings;

/**
 * Struct that stores information about status of InteractionComponent.
 */
USTRUCT()
struct FInteractionComponentInfo
{
	GENERATED_USTRUCT_BODY()

	FInteractionComponentInfo()
		: InteractionComponent(nullptr)
		, InteractionAction(nullptr)
	{
	}

	FInteractionComponentInfo(UInteractionComponent* InInteractionComponent, UBaseInteractionAction* InInteractionAction)
		: InteractionComponent(InInteractionComponent)
		, InteractionAction(InInteractionAction)
	{
	}

	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY()
	TObjectPtr<UBaseInteractionAction> InteractionAction;
};

DECLARE_MULTICAST_DELEGATE(FOnInteractionSubsystemInitializedDelegate);

/**
 * Subsystem that provides binding and checks for all interactions between
 * Interaction and Interacting components.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UPlayerInteractionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	bool IsInitialized() const { return bIsInitialized; }

	void RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);
	void RemoveInteraction(UInteractingComponent* InteractingComponent);
	void RemoveInteraction(UInteractionComponent* InteractionComponent);

	bool IsInteractionRegistered(const UInteractingComponent* InteractingComponent) const;
	bool IsInteractionRegistered(const UInteractionComponent* InteractionComponent) const;

	bool IsInteracting(const UInteractingComponent* InteractingComponent) const;
	bool IsInteracting(const UInteractionComponent* InteractionComponent) const;

	const UBaseInteractionAction* GetCurrentInterractionAction(const UInteractingComponent* InteractingComponent) const;

	FOnInteractionSubsystemInitializedDelegate& GetOnSubsystemInitializedDelegate() { return OnInteractionSubsystemInitializedDelegate; }
	
	/**
	* Allow to start interaction action with interaction component for interacting component, 
	* if it's already registered in this subsystem.
	*
	* @param InteractingComponent - pointer to component that request interaction.
	* @return true if action is continuous and can be stopped after activation.
	*/ 
	bool StartInteractionAction(UInteractingComponent* InteractingComponent);

	/**
	* Allow to stop interaction action with interaction component, if continuous action 
	* was started earlier for this component.
	*
	* @param InteractingComponent - pointer to component that request interaction.
	* @return true if action was stopped successfully.
	*/
	bool StopInteractionAction(UInteractingComponent* InteractingComponent);
	
	bool StopInteractionAction(UInteractionComponent* InteractionComponent);

private:
	bool TryToActivateAutomaticAction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent);
	
	UInteractingComponent* FindBoundInteractingComponent(const UInteractionComponent* InteractionComponent) const;

	UBaseInteractionAction* CreateInteractionAction(UInteractionComponent* CurrentInteractionComponent);

	void OnSettingsLoaded();

private:
	TMap<UInteractingComponent*, UInteractionComponent*> RegisteredInteractionsMap;

	UPROPERTY()
	TMap<UInteractingComponent*, FInteractionComponentInfo> ActiveInteractionsMap;

private:
	bool bIsInitialized = false;
	FOnInteractionSubsystemInitializedDelegate OnInteractionSubsystemInitializedDelegate;

	UPROPERTY()
	TObjectPtr<const UInteractionSubsystemSettings> InteractionSettings;
};

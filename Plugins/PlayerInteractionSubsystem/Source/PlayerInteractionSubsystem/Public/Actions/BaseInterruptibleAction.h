#pragma once

#include "Actions/BaseInteractionAction.h"
#include "Engine/TimerHandle.h"
#include "BaseInterruptibleAction.generated.h"

class UBaseInteractionActionDataAsset;

/**
 * Base class for interruptible interaction actions.
 * It provides functionality for actions that can be activated and interrupted during player interaction.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UBaseInterruptibleAction : public UBaseInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Initialize(const UBaseInteractionActionDataAsset* InteractionActionDataAsset) override;
	virtual void BeginDestroy() override;

	virtual bool Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;
	virtual bool Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;

	virtual FText GetActionDescription() const override;
	
	float GetTotalActivationTime() const { return ActivationTime; }
	float GetRemainingActivationTime() const;

protected:
	virtual void OnActivationStarted() {};
	virtual void OnDeactivationStarted() {};

	virtual void OnActivationCompleted() {};
	virtual void OnActivationInterrupted() {};

private:
	void InterruptActivation();
	void CompleteActivation();

	void OnInteractionRegisterStateChanged(bool bIsInteracting, const UInteractionComponent* InteractionComponent);

private:
	float ActivationTime = 1.f;

	FText InterruptionCaption;

private:
	// Stores original value of bShouldBeDeactivated. It is needed to set that it can be deactivated during activation.
	bool bShouldBeDeactivatedAfterActivation;

	FTimerHandle ActivationTimerHandle;
	bool bIsActivationInProgress = false;
};

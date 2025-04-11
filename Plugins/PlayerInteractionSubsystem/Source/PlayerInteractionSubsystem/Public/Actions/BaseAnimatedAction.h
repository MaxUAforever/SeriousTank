#pragma once

#include "Actions/BaseInteractionAction.h"
#include "BaseAnimatedAction.generated.h"

class UAnimInstance;
class UAnimMontage;
class UBaseInteractionActionDataAsset;

/**
 * Base class for animated interaction actions.
 * This class provides functionality for actions that should use animation montages
 * for activation and deactivation and would be activated/deactivated after playing it.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UBaseAnimatedAction : public UBaseInteractionAction
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;
	
	virtual void Initialize(const UBaseInteractionActionDataAsset* InteractionActionDataAsset) override;

	virtual bool Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;
	virtual bool Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;

protected:
	virtual void OnActivationStarted() {};
	virtual void OnDeactivationStarted() {};

	virtual void OnActivationMontagePlayed() {};
	virtual void OnDeactivationMontagePlayed() {};

private:
	void PlayActivationMontage();
	void PlayDeactivationMontage();

	void PlayMontage_Internal(UAnimMontage* AnimMontage, float Delay, TFunction<void()> OnMontageCompleted);
	UAnimInstance* GetOwnerAnimInstance();

private:
	float ActivationMontageCompleteDelay = 0.f;
	float DeactivationMontageCompleteDelay = 0.f;

	FTimerHandle ActionTimerHandle;
};

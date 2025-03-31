#pragma once

#include "Actions/BaseAnimatedAction.h"
#include "PossessVehicleAction.generated.h"

class UAnimMontage;
class UInputAction;
class UInputMappingContext;

/**
 * Action for possessing a vehicle actor if pawn with needed interactiong component called it.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UPossessVehicleAction : public UBaseAnimatedAction
{
	GENERATED_BODY()

public:
	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const override;

protected:
	virtual void OnActivationStarted() override;
	virtual void OnDeactivationStarted() override;

	virtual void OnActivationMontagePlayed() override;
	virtual void OnDeactivationMontagePlayed() override;

private:
	TObjectPtr<APawn> CachedCharacterPawn;
	TObjectPtr<APawn> CachedVehiclePawn;
	
	TOptional<FTransform> CachedInteractionPoint;
};

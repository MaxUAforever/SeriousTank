#pragma once

#include "Actions/BaseInteractionAction.h"
#include "PossessVehicleAction.generated.h"

class UAnimMontage;
class UInputAction;
class UInputMappingContext;
class UInteractionPointComponent;
/**
 * Action for possessing a vehicle actor if pawn with needed interactiong component called it.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UPossessVehicleAction : public UBaseInteractionAction
{
	GENERATED_BODY()

public:
	UPossessVehicleAction();

	// Function for activation of possessing vehicle interaction.
	virtual bool Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;
	
	// Function to start unpossessing of vehicle.
	virtual bool Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;

	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const override;
	virtual FText GetActionDescription() const override;

private:
	void PlayActionMontage(APawn* CharacterPawn, APawn* VehiclePawn) const;
	void PlayDeactivationMontage(APawn* CharacterPawn, APawn* VehiclePawn, UInteractionComponent* InteractionComponent, const UInteractionPointComponent* InteractionPoint) const;

	void PlayMontage_Internal(APawn* CharacterPawn, APawn* VehiclePawn, UAnimMontage* AnimMontage, float Delay, TFunction<void()> OnMontageCompleted) const;
};

#include "Actions/PossessVehicleAction.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InteractingComponent.h"
#include "InteractionComponent.h"

void UPossessVehicleAction::Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!CanBeActivated(InteractingComponent, InteractionComponent))
	{
		return;
	}

	APawn* CharacterPawn = Cast<APawn>(InteractingComponent->GetOwner());
	APawn* VehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());
	AController* CharacterController = CharacterPawn->GetController();

	CharacterPawn->GetRootComponent()->SetVisibility(false, true);
	CharacterPawn->SetActorEnableCollision(false);
	
	// TODO: add SetViewTargetWithBlend for new camera
	CharacterController->Possess(VehiclePawn);
}

bool UPossessVehicleAction::CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!InteractingComponent || !InteractionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPossessVehicleAction::Activate: Failed to get InteractingComponent or InteractionComponent"));
		return false;
	}

	APawn* CharacterPawn = Cast<APawn>(InteractingComponent->GetOwner());
	APawn* VehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());
	if (!CharacterPawn || !VehiclePawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPossessVehicleAction::Activate: Owners of components are not pawns."));
		return false;
	}

	AController* CharacterController = CharacterPawn->GetController();
	if (CharacterController == nullptr || VehiclePawn->GetController() != nullptr)
	{
		return false;
	}

	return true;
}

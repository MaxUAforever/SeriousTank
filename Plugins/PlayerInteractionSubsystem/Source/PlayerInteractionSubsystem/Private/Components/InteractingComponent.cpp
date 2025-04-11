#include "Components/InteractingComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "Data/InteractionSubsystemSettings.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "PlayerInteractionCoreTypes.h"
#include "PlayerInteractionSubsystem.h"

void UInteractingComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UST_InteractingComponent::BeginPlay: Owner of component is not a pawn."));
		return;
	}

	PlayerInteractionSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UPlayerInteractionSubsystem>() : nullptr;
	if (IsValid(PlayerInteractionSubsystem))
	{
		auto OnSubsystemInitialized = [this, OwnerPawn]()
		{
			OnInteractionActionBoundDelegate.Broadcast();
		};

		if (PlayerInteractionSubsystem->IsInitialized())
		{
			OnSubsystemInitialized();
		}
		else
		{
			PlayerInteractionSubsystem->GetOnSubsystemInitializedDelegate().AddLambda(OnSubsystemInitialized);
		}
	}
}

bool UInteractingComponent::StartInteraction()
{
	if (!PlayerInteractionSubsystem)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractingComponent::Interact: failed to get PlayerInteractionSubsystem"));
		return false;
	}

	if (!bIsInteractionEnabled)
	{
		return false;
	}

	if (PlayerInteractionSubsystem->IsInteracting(this))
	{
		return false;
	}

	bool bDeactivatingInteractionStarted = PlayerInteractionSubsystem->StartInteractionAction(this);
	if (bDeactivatingInteractionStarted)
	{
		OnDeactivatingActionStartedDelegate.Broadcast();
	}

	return bDeactivatingInteractionStarted;
}

bool UInteractingComponent::StopInteraction()
{
	if (!PlayerInteractionSubsystem)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractingComponent::Interact: failed to get PlayerInteractionSubsystem"));
		return false;
	}

	return PlayerInteractionSubsystem->StopInteractionAction(this);
}

bool UInteractingComponent::IsInteracting() const
{
	if (!IsValid(PlayerInteractionSubsystem))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractingComponent::IsInteracting: failed to get PlayerInteractionSubsystem"));
		return false;
	}

	return PlayerInteractionSubsystem->IsInteracting(this);
}

bool UInteractingComponent::IsInteractingBlockingly() const
{
	if (!IsValid(PlayerInteractionSubsystem))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractingComponent::IsInteracting: failed to get PlayerInteractionSubsystem"));
		return false;
	}

	const UBaseInteractionAction* CurrentAction = PlayerInteractionSubsystem->GetCurrentInterractionAction(this);
	if (!IsValid(CurrentAction))
	{
		return false;
	}

	return CurrentAction->IsBlockingInteraction();
}

void UInteractingComponent::SetInteractionEnabled(bool bInIsInteractionEnabled)
{
	bIsInteractionEnabled = bInIsInteractionEnabled;

	StopInteraction();
}

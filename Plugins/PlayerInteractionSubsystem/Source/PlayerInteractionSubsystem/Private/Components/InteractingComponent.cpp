#include "Components/InteractingComponent.h"

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
			OwnerPawn->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnControllerChanged);
			OnControllerChanged(OwnerPawn, nullptr, OwnerPawn->GetController());
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

void UInteractingComponent::Interact()
{
	if (!PlayerInteractionSubsystem)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractingComponent::Interact: failed to get PlayerInteractionSubsystem"));
		return;
	}

	if (!bIsInteracting)
	{
		bIsInteracting = PlayerInteractionSubsystem->StartInteractionAction(this);
	}
}

void UInteractingComponent::StopInteraction()
{
	if (!PlayerInteractionSubsystem)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractingComponent::Interact: failed to get PlayerInteractionSubsystem"));
		return;
	}

	if (bIsInteracting)
	{
		bIsInteracting = !PlayerInteractionSubsystem->StopInteractionAction(this);
	}
}

void UInteractingComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UST_InteractingComponent::BeginPlay: Owner of component is not a pawn."));
		return;
	}

	APlayerController* PC = NewController ? Cast<APlayerController>(NewController) : Cast<APlayerController>(OldController);
	if (!PC)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UST_InteractingComponent::BeginPlay: Failed to get possessing player controller of parent pawn."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedComponent = PC->GetComponentByClass<UEnhancedInputComponent>();
	if (!EnhancedSubsystem)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UST_InteractingComponent::BeginPlay: Failed to get EnhancedSubsystem or EnhancedComponent of owning actor."));
		return;
	}

	const UInteractionSubsystemSettings* InteractionSettings = GetDefault<UInteractionSubsystemSettings>();
	if (NewController && EnhancedComponent)
	{
		EnhancedComponent->BindAction(InteractionSettings->InteractInputAction.Get(), ETriggerEvent::Started, this, &ThisClass::Interact);
		
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bForceImmediately = true;
		EnhancedSubsystem->AddMappingContext(InteractionSettings->InteractionInputContext.Get(), 0, ModifyContextOptions);

		OnInteractionActionBoundDelegate.Broadcast();
	}
	else
	{
		EnhancedSubsystem->RemoveMappingContext(InteractionSettings->InteractionInputContext.Get());
	}
}

#include "InteractingComponent.h"

#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "PlayerInteractionSubsystem.h"

void UInteractingComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_InteractingComponent::BeginPlay: Owner of component is not a pawn."));
		return;
	}

	OwnerPawn->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnControllerChanged);
	OnControllerChanged(OwnerPawn, nullptr, OwnerPawn->GetController());
}

void UInteractingComponent::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("UInteractingComponent::Interact called"));

	if (PlayerInteractionSubsystem)
	{
		PlayerInteractionSubsystem->StartInteractionAction(this);
	}
}

void UInteractingComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (!InteractionInputContext || !InteractInputAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_InteractingComponent::BeginPlay: Input data is not fully specified for class."));
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_InteractingComponent::BeginPlay: Owner of component is not a pawn."));
		return;
	}

	APlayerController* PC = NewController ? Cast<APlayerController>(NewController) : Cast<APlayerController>(OldController);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_InteractingComponent::BeginPlay: Failed to get possessing player controller of parent pawn."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedComponent = PC->GetComponentByClass<UEnhancedInputComponent>();
	if (!EnhancedSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_InteractingComponent::BeginPlay: Failed to get EnhancedSubsystem or EnhancedComponent of owning actor."));
		return;
	}

	if (NewController && EnhancedComponent)
	{
		PlayerInteractionSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UPlayerInteractionSubsystem>() : nullptr;

		EnhancedComponent->BindAction(InteractInputAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bForceImmediately = true;
		EnhancedSubsystem->AddMappingContext(InteractionInputContext, 0, ModifyContextOptions);

		OnInteractionActionBoundDelegate.Broadcast();
	}
	else
	{
		EnhancedSubsystem->RemoveMappingContext(InteractionInputContext);
	}
}

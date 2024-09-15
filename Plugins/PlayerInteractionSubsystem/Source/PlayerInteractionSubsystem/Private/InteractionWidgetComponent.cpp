#include "InteractionWidgetComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractingComponent.h"
#include "InteractionComponent.h"
#include "InteractionUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UInteractionWidgetComponent::UpdateWidgetData()
{
	InteractionWidget = Cast<UInteractionUserWidget>(GetUserWidgetObject());
	if (!InteractionWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionWidgetComponent::BeginPlay: failed to get Widget"));
		return;
	}

	const UInteractionComponent* InteractionComponent = GetOwner()->GetComponentByClass<UInteractionComponent>();
	if (!InteractionComponent || !InteractionComponent->GetAction())
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionWidgetComponent::BeginPlay: failed to get InteractionComponent"));
		return;
	}

	InteractionWidget->SetActionNameCaption(InteractionComponent->GetAction()->GetActionDescription());

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PlayerController) || !IsValid(PlayerController->GetPawn()))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedComponent = PlayerController->GetComponentByClass<UEnhancedInputComponent>();
	if (!EnhancedSubsystem || !EnhancedComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_InteractingComponent::BeginPlay: Failed to get EnhancedSubsystem or EnhancedComponent of owning actor."));
		return;
	}
	
	UInteractingComponent* InteractingComponent = PlayerController->GetPawn()->GetComponentByClass<UInteractingComponent>();
	if (!InteractingComponent)
	{
		return;
	}

	auto TryToSetButtonCaption = [this, InteractingComponent, EnhancedSubsystem]()
	{
		if (!InteractingComponent || !EnhancedSubsystem)
		{
			return false;
		}

		const TArray<FKey> ActionKeys = EnhancedSubsystem->QueryKeysMappedToAction(InteractingComponent->GetInputAction());
		if (ActionKeys.Num() > 0)
		{
			InteractionWidget->SetInteractionButtonCaption(ActionKeys[0].GetDisplayName(false));
			return true;
		}

		return false;
	};

	if (!TryToSetButtonCaption())
	{
		InteractingComponent->OnInteractionActionBoundDelegate.AddLambda([TryToSetButtonCaption]() { TryToSetButtonCaption(); });
	}
	
}
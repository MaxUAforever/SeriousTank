#include "Components/InteractionWidgetComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "Data/Actions/BaseInteractionActionDataAsset.h"
#include "Data/InteractionSubsystemSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractionUserWidget.h"

void UInteractionWidgetComponent::UpdateWidgetData()
{
	InteractionWidget = Cast<UInteractionUserWidget>(GetUserWidgetObject());
	if (!InteractionWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionWidgetComponent::BeginPlay: failed to get Widget"));
		return;
	}

	const UInteractionComponent* InteractionComponent = GetOwner()->GetComponentByClass<UInteractionComponent>();
	if (!InteractionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionWidgetComponent::BeginPlay: failed to get InteractionComponent"));
		return;
	}

	const UBaseInteractionActionDataAsset* InteractionActionDataAsset = InteractionComponent->GetInteractionActionDataAsset();
	if (!IsValid(InteractionActionDataAsset))
	{
		return;
	}
	InteractionWidget->SetActionNameCaption(InteractionActionDataAsset->ActionDescription);

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

	auto TryToSetButtonCaption = [this, EnhancedSubsystem]()
	{
		if (!EnhancedSubsystem)
		{
			return false;
		}
		
		const UInteractionSubsystemSettings* InteractionSettings = GetDefault<UInteractionSubsystemSettings>();
		if (!InteractionSettings->InteractInputAction.IsValid())
		{
			return false;
		}

		const TArray<FKey> ActionKeys = EnhancedSubsystem->QueryKeysMappedToAction(InteractionSettings->InteractInputAction.Get());
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
#include "Components/InteractionWidgetComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "Data/Actions/BaseInteractionActionDataAsset.h"
#include "Data/InteractionSubsystemSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInteractionCoreTypes.h"
#include "UI/InteractionUserWidget.h"

void UInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	UInteractionComponent* InteractionComponent = GetOwner()->GetComponentByClass<UInteractionComponent>();
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->OnInteractionActionStartedDelegate.AddUObject(this, &ThisClass::OnInteractionActionStarted);
		InteractionComponent->OnInteractionActionStoppedDelegate.AddUObject(this, &ThisClass::OnInteractionActionStopped);
	}
}

void UInteractionWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UInteractionComponent* InteractionComponent = GetOwner()->GetComponentByClass<UInteractionComponent>();
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->OnInteractionActionStartedDelegate.RemoveAll(this);
		InteractionComponent->OnInteractionActionStoppedDelegate.RemoveAll(this);
	}
}

void UInteractionWidgetComponent::UpdateWidgetData()
{
	InteractionWidget = Cast<UInteractionUserWidget>(GetUserWidgetObject());
	if (!InteractionWidget)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: failed to get Widget"));
		return;
	}

	const UInteractionComponent* InteractionComponent = GetOwner()->GetComponentByClass<UInteractionComponent>();
	if (!InteractionComponent)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: failed to get InteractionComponent"));
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
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: Failed to get EnhancedSubsystem or EnhancedComponent of owning actor."));
		return;
	}
	
	UInteractingComponent* InteractingComponent = PlayerController->GetPawn()->GetComponentByClass<UInteractingComponent>();
	if (!InteractingComponent)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: Failed to get InteractingComponent."));
		return;
	}

	auto TryToSetButtonCaption = [this, EnhancedSubsystem]()
	{
		if (!EnhancedSubsystem)
		{
			UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: Failed to get EnhancedSubsystem."));
			return false;
		}
		
		const UInteractionSubsystemSettings* InteractionSettings = GetDefault<UInteractionSubsystemSettings>();
		if (!InteractionSettings->InteractInputAction.IsValid())
		{
			UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: Failed to get InteractInputAction."));
			return false;
		}

		const TArray<FKey> ActionKeys = EnhancedSubsystem->QueryKeysMappedToAction(InteractionSettings->InteractInputAction.Get());
		if (ActionKeys.Num() <= 0)
		{
			UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("UInteractionWidgetComponent::UpdateWidgetData: Failed to get ActionKeys."));
			return false;
		}
			
		InteractionWidget->SetInteractionButtonCaption(ActionKeys[0].GetDisplayName(false));
		return true;
	};

	if (!TryToSetButtonCaption())
	{
		InteractingComponent->OnInteractionActionBoundDelegate.AddLambda([TryToSetButtonCaption]() { TryToSetButtonCaption(); });
	}
}

void UInteractionWidgetComponent::OnInteractionActionStarted(UBaseInteractionAction* InteractionAction)
{
	if (InteractionWidget)
	{
		InteractionWidget->SetActionNameCaption(InteractionAction->GetActionDescription());
		InteractionWidget->OnInteractionActionStarted(InteractionAction);
	}
}

void UInteractionWidgetComponent::OnInteractionActionStopped()
{
	if (InteractionWidget)
	{
		UpdateWidgetData();
		InteractionWidget->OnInteractionActionStopped();
	}
}

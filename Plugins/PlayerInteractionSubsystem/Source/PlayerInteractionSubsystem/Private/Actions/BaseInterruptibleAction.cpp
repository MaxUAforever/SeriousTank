#include "Actions/BaseInterruptibleAction.h"

#include "Data/Actions/BaseInterruptibleActionDataAsset.h"
#include "Engine/World.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"
#include "PlayerInteractionSubsystem.h"

void UBaseInterruptibleAction::Initialize(const UBaseInteractionActionDataAsset* InteractionActionDataAsset)
{
	Super::Initialize(InteractionActionDataAsset);

	const UBaseInterruptibleActionDataAsset* InterruptibleActionDataAsset = Cast<UBaseInterruptibleActionDataAsset>(InteractionActionDataAsset);
	if (!IsValid(InterruptibleActionDataAsset))
	{
		return;
	}

	ActivationTime = InterruptibleActionDataAsset->ActivationTime;
	InterruptionCaption = InterruptibleActionDataAsset->InterruptionCaption;

	bShouldBeDeactivatedAfterActivation = ShouldBeDeactivated();
}

void UBaseInterruptibleAction::BeginDestroy()
{
	Super::BeginDestroy();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ActivationTimerHandle);
	}

	UInteractingComponent* InteractingComponent = GetInteractingComponent();
	if (IsValid(InteractingComponent))
	{
		InteractingComponent->OnInteractionRegisterStateChangedDelegate.RemoveAll(this);
	}
}

bool UBaseInterruptibleAction::Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!Super::Activate(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	if (bIsActivationInProgress)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	bIsActivationInProgress = true;
	World->GetTimerManager().SetTimer(ActivationTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::CompleteActivation), ActivationTime, false);

	SetShouldBeDeactivated(true);
	OnActivationStarted();

	InteractingComponent->OnInteractionRegisterStateChangedDelegate.AddUObject(this, &ThisClass::OnInteractionRegisterStateChanged);
	InteractionComponent->UpdateWidgetData();

	return true;
}

bool UBaseInterruptibleAction::Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (bIsActivationInProgress)
	{
		InterruptActivation();
		return true;
	}

	OnDeactivationStarted();
	return true;
}

FText UBaseInterruptibleAction::GetActionDescription() const
{
	return bIsActivationInProgress ? InterruptionCaption : GetDefaultActionDescription();
}

float UBaseInterruptibleAction::GetRemainingActivationTime() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return ActivationTime;
	}

	return bIsActivationInProgress ? World->GetTimerManager().GetTimerRemaining(ActivationTimerHandle) : ActivationTime;
}

void UBaseInterruptibleAction::InterruptActivation()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(ActivationTimerHandle);
	bIsActivationInProgress = false;

	SetShouldBeDeactivated(bShouldBeDeactivatedAfterActivation);

	UInteractionComponent* InteractionComponent = GetInteractionComponent();
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->UpdateWidgetData();
	}

	OnActivationInterrupted();
}

void UBaseInterruptibleAction::CompleteActivation()
{
	bIsActivationInProgress = false;
	SetShouldBeDeactivated(bShouldBeDeactivatedAfterActivation);

	UWorld* World = GetWorld();
	if (IsValid(World) && !bShouldBeDeactivatedAfterActivation)
	{
		UPlayerInteractionSubsystem* PlayerInteractionSubsystem = World->GetSubsystem<UPlayerInteractionSubsystem>();
		if (IsValid(PlayerInteractionSubsystem))
		{
			PlayerInteractionSubsystem->StopInteractionAction(GetInteractionComponent());
		}
	}

	if (IsValid(GetActivationMontage()))
	{

	}

	OnActivationCompleted();
}

void UBaseInterruptibleAction::OnInteractionRegisterStateChanged(bool bIsInteracting, const UInteractionComponent* InteractionComponent)
{
	if (!bIsInteracting && bIsActivationInProgress)
	{
		InterruptActivation();
	}
}

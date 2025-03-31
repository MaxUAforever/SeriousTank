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
	DeactivationTime = InterruptibleActionDataAsset->DeactivationTime;
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
		InteractingComponent->OnInteractingStateChanged.RemoveAll(this);
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
		InterruptActivation();
		return true;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	bIsActivationInProgress = true;
	World->GetTimerManager().SetTimer(ActivationTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::CompleteActivation), ActivationTime, false);

	SetShouldBeDeactivated(true);

	InteractingComponent->OnInteractingStateChanged.AddUObject(this, &ThisClass::OnInteractingStateChanged);
	InteractionComponent->UpdateWidgetData();

	return true;
}

FText UBaseInterruptibleAction::GetActionDescription() const
{
	return bIsActivationInProgress ? InterruptionCaption : GetDefaultActionDescription();
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
			PlayerInteractionSubsystem->StopInteractionAction(GetInteractingComponent());
		}
	}

	OnActivationCompleted();
}

void UBaseInterruptibleAction::OnInteractingStateChanged(bool bIsInteracting)
{
	if (!bIsInteracting && bIsActivationInProgress)
	{
		InterruptActivation();
	}
}

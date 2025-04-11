#include "UI/InterruptibleInteractionUserWidget.h"

#include "Actions/BaseInterruptibleAction.h"
#include "Components/ProgressBar.h"

void UInterruptibleInteractionUserWidget::OnInteractionActionStarted(UBaseInteractionAction* InteractionAction)
{
	CurrentAction = Cast<UBaseInterruptibleAction>(InteractionAction);
	if (!IsValid(CurrentAction))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	World->GetTimerManager().SetTimer(ProgressUpdateTimerHandle, this, &ThisClass::UpdateActivationProgress, 0.1f, true);
}

void UInterruptibleInteractionUserWidget::OnInteractionActionStopped()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);
	}

	if (IsValid(ActivationProgressBar))
	{
		ActivationProgressBar->SetPercent(0.f);
	}
}

void UInterruptibleInteractionUserWidget::BeginDestroy()
{
	Super::BeginDestroy();

	OnInteractionActionStopped();
}

void UInterruptibleInteractionUserWidget::UpdateActivationProgress()
{
	const float Progress = (CurrentAction->GetTotalActivationTime() - CurrentAction->GetRemainingActivationTime()) / CurrentAction->GetTotalActivationTime();
	ActivationProgressBar->SetPercent(FMath::Clamp(Progress, 0.f, 1.f));
}
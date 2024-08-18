#include "PlayerInteractionSubsystem.h"

#include "Actions/BaseInteractionAction.h"
#include "InteractingComponent.h"
#include "InteractionComponent.h"

void UPlayerInteractionSubsystem::RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!InteractingComponent || !InteractionComponent || !InteractionComponent->GetAction())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionSubsystem::RegisterInteraction: failed to get InteractingComponent or InteractionComponent"));
		return;
	}

	const bool bWasEmpty = InteractionMap.Num() == 0;

	const bool CanBeActivated = InteractionComponent->GetAction()->CanBeActivated(InteractingComponent, InteractionComponent);
	InteractionMap.Add(InteractingComponent, { InteractionComponent, CanBeActivated });
	InteractingComponent->OnInteractingStateChanged.Broadcast(true);

	if (bWasEmpty && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(InteractionsUpdateTimerHandle, RefreshRate, true);
	}
}

void UPlayerInteractionSubsystem::RemoveInteraction(UInteractingComponent* InteractingComponent)
{
	InteractionMap.Remove(InteractingComponent);
	InteractingComponent->OnInteractingStateChanged.Broadcast(false);

	if (InteractionMap.Num() == 0 && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractionsUpdateTimerHandle);
	}
}

void UPlayerInteractionSubsystem::RemoveInteraction(UInteractionComponent* InteractingComponent)
{
	for (auto InteractionMapIt = InteractionMap.CreateConstIterator(); InteractionMapIt; ++InteractionMapIt)
	{
		UInteractingComponent* InteractingComponent = InteractionMapIt.Key();
		if (!InteractingComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionSubsystem::RemoveInteraction: failed to get InteractingComponen"));
			continue;
		}

		RemoveInteraction(InteractingComponent);
	}
}

bool UPlayerInteractionSubsystem::StartInteractionAction(UInteractingComponent* InteractingComponent)
{
	InteractionComponentInfo* InteractionComponentInfo = InteractionMap.Find(InteractingComponent);
	if (!InteractionComponentInfo || !InteractionComponentInfo->InteractionComponent)
	{
		return false;
	}

	const bool bSuccessfulAction = InteractionComponentInfo->InteractionComponent->ActivateAction(InteractingComponent);
	const UBaseInteractionAction* Action = InteractionComponentInfo->InteractionComponent->GetAction();
	if (bSuccessfulAction && Action && Action->ShouldBeDeactivated())
	{
		ActiveInteractionsMap.Add(InteractingComponent, InteractionComponentInfo->InteractionComponent);

		return Action->ShouldBeDeactivated();
	}

	return false;
}

bool UPlayerInteractionSubsystem::StopInteractionAction(UInteractingComponent* InteractingComponent)
{
	UInteractionComponent** InteractionComponent = ActiveInteractionsMap.Find(InteractingComponent);
	if (!InteractionComponent || !(*InteractionComponent))
	{
		return false;
	}

	const bool bSuccess = (*InteractionComponent)->DeactivateAction(InteractingComponent);
	if (bSuccess)
	{
		ActiveInteractionsMap.Remove(InteractingComponent);
	}

	return bSuccess;
}

//void UPlayerInteractionSubsystem::UpdateInteractionStatuses()
//{
//	for (TTuple<UInteractingComponent*, InteractionComponentInfo> InteractionInfo : InteractionMap)
//	{
//		UInteractingComponent* InteractingComponent = InteractionInfo.Key;
//		UInteractionComponent* InteractionComponent = InteractionInfo.Value.InteractionComponent;
//		if (!InteractionComponent || !InteractionComponent->GetAction())
//		{
//			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionSubsystem::UpdateInteractionStatuses: failed to get InteractionComponent"));
//			continue;
//		}
//
//		const bool bNewCanBeActivated = InteractionComponent->GetAction()->CanBeActivated(InteractingComponent, InteractionComponent);
//		if (InteractionInfo.Value.bCanBeActivated != bNewCanBeActivated)
//		{
//			InteractionInfo.Value.bCanBeActivated = bNewCanBeActivated;
//			InteractingComponent->OnInteractingStateChanged.Broadcast(bNewCanBeActivated);
//		}
//	}
//}

#include "PlayerInteractionSubsystem.h"

#include "Actions/BaseInteractionAction.h"
#include "Data/InteractionSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"
#include "PlayerInteractionCoreTypes.h"

void UPlayerInteractionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InteractionSettings = GetDefault<UInteractionSubsystemSettings>();
	
	TArray<FSoftObjectPath> AssetsToLoad;
	if (InteractionSettings->InteractionInputContext.IsPending())
	{
		AssetsToLoad.Add(InteractionSettings->InteractionInputContext.ToSoftObjectPath());
	}
	if (InteractionSettings->InteractInputAction.IsPending())
	{
		AssetsToLoad.Add(InteractionSettings->InteractInputAction.ToSoftObjectPath());
	}

	if (!AssetsToLoad.IsEmpty())
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &ThisClass::OnSettingsLoaded));
	}
	else
	{
		OnSettingsLoaded();
	}
}

void UPlayerInteractionSubsystem::RegisterInteraction(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!InteractingComponent || !InteractionComponent || !InteractionComponent->GetActionClass())
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::RegisterInteraction: failed to get InteractingComponent or InteractionComponent"));
		return;
	}

	RegisteredInteractionsMap.Add(InteractingComponent, InteractionComponent);
	InteractingComponent->OnInteractingStateChanged.Broadcast(true);
}

void UPlayerInteractionSubsystem::RemoveInteraction(UInteractingComponent* InteractingComponent)
{
	if (!IsValid(InteractingComponent) || ActiveInteractionsMap.Contains(InteractingComponent))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::RemoveInteraction: failed to get InteractingComponent or InteractionComponent"));
		return;
	}

	RegisteredInteractionsMap.Remove(InteractingComponent);
	InteractingComponent->OnInteractingStateChanged.Broadcast(false);
}

void UPlayerInteractionSubsystem::RemoveInteraction(UInteractionComponent* InteractingComponent)
{
	if (!IsValid(InteractingComponent))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::RemoveInteraction: failed to get InteractingComponent"));
		return;
	}

	for (auto InteractionMapIt = ActiveInteractionsMap.CreateConstIterator(); InteractionMapIt; ++InteractionMapIt)
	{
		UInteractingComponent* InteractingComponent = InteractionMapIt.Key();
		if (!InteractingComponent)
		{
			continue;
		}

		RemoveInteraction(InteractingComponent);
	}
}

bool UPlayerInteractionSubsystem::StartInteractionAction(UInteractingComponent* InteractingComponent)
{
	UInteractionComponent** CurrentInteractionComponentPtr = RegisteredInteractionsMap.Find(InteractingComponent);
	UInteractionComponent* CurrentInteractionComponent = CurrentInteractionComponentPtr ? *CurrentInteractionComponentPtr : nullptr;
	if (!IsValid(CurrentInteractionComponent) || !CurrentInteractionComponent->IsInteractionComponentActive() || !CurrentInteractionComponent->GetActionClass())
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::StartInteractionAction: failed to get valid CurrentInteractionComponent or it's not active"));
		return false;
	}

	TSet<AActor*> OverlappingActors;
	CurrentInteractionComponent->GetOverlappingActors(OverlappingActors);
	if (!OverlappingActors.Contains(InteractingComponent->GetOwner()))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::StartInteractionAction: Owner of InteractingComponent doesn't overlap InteractionComponent"));
		
		RemoveInteraction(InteractingComponent);
		return false;
	}

	UBaseInteractionAction* InteractionAction = NewObject<UBaseInteractionAction>(this, CurrentInteractionComponent->GetActionClass());
	if (!IsValid(InteractionAction))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::StartInteractionAction: Failed to create InteractionAction."));
		return false;
	}

	InteractionAction->Initialize(CurrentInteractionComponent->GetInteractionActionDataAsset());

	ActiveInteractionsMap.Add(InteractingComponent, { CurrentInteractionComponent, InteractionAction });
	const bool bSuccessfulAction = InteractionAction->Activate(InteractingComponent, CurrentInteractionComponent);
	if (!bSuccessfulAction || !InteractionAction->ShouldBeDeactivated())
	{
		ActiveInteractionsMap.Remove(InteractingComponent);
		return false;
	}
		
	return InteractionAction->ShouldBeDeactivated();
}

bool UPlayerInteractionSubsystem::StopInteractionAction(UInteractingComponent* InteractingComponent)
{
	FInteractionComponentInfo* InteractionComponentInfo = ActiveInteractionsMap.Find(InteractingComponent);
	if (!InteractionComponentInfo)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::StopInteractionAction: Failed to get InteractionComponentInfo."));
		return false;
	}

	UInteractionComponent* CurrentInteractionComponent = InteractionComponentInfo->InteractionComponent;
	UBaseInteractionAction* CurrentInteractionAction = InteractionComponentInfo->InteractionAction;
	if (!IsValid(CurrentInteractionComponent) || !IsValid(CurrentInteractionAction))
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("PlayerInteractionSubsystem::StopInteractionAction: Failed to get CurrentInteractionComponent or CurrentInteractionAction."));
		return false;
	}

	const bool bSuccess = CurrentInteractionAction->Deactivate(InteractingComponent, CurrentInteractionComponent);
	if (bSuccess)
	{
		ActiveInteractionsMap.Remove(InteractingComponent);
	}

	return bSuccess;
}

void UPlayerInteractionSubsystem::OnSettingsLoaded()
{
	if (!InteractionSettings->InteractionInputContext.IsValid() || !InteractionSettings->InteractInputAction.IsValid())
	{
		UE_LOG(LogPlayerInteractionSubsystem, Error, TEXT("PlayerInteractionSubsystem::OnSettingsLoaded: Failed to load Settings for plugin."));
		return;
	}

	bIsInitialized = true;
	OnInteractionSubsystemInitializedDelegate.Broadcast();
}

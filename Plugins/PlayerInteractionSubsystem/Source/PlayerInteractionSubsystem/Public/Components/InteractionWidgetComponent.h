#pragma once

#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

class UBaseInteractionAction;
class UInteractionUserWidget;

/**
 * Component that allows to control widget for interaction.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Function that allows to refresh all info for attached interaction widget.
	void UpdateWidgetData();

private:
	void OnInteractionActionStarted(UBaseInteractionAction* InteractionAction);
	void OnInteractionActionStopped();

private:
	TObjectPtr<UInteractionUserWidget> InteractionWidget;
};

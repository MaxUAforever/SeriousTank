#pragma once

#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

class UInteractionUserWidget;

/**
 * Component that allows to control widget for interaction.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	// Function that allows to refresh all info for attached interaction widget.
	void UpdateWidgetData();

private:
	UInteractionUserWidget* InteractionWidget;
};

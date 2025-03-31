#pragma once

#include "Engine/DeveloperSettings.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InteractionSubsystemSettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Interaction Subsystem Settings"))
class PLAYERINTERACTIONSUBSYSTEM_API UInteractionSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, Category = "Input", EditAnywhere, AdvancedDisplay)
	TSoftObjectPtr<UInputMappingContext> InteractionInputContext;

	UPROPERTY(Config, Category = "Input", EditAnywhere, AdvancedDisplay)
	TSoftObjectPtr<UInputAction> InteractInputAction;
};

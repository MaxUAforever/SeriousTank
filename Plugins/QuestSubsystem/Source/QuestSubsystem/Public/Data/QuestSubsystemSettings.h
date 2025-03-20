#pragma once

#include "Engine/DeveloperSettings.h"
#include "QuestSubsystemSettings.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Quest Subsystem Settings"))
class QUESTSUBSYSTEM_API UQuestSubsystemSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Data", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> QuestsDataTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Data", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> QuestTasksDataTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Data", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> TaskStartConditionsDataTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Data", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> TaskWidgetsDataTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
	bool bShouldTrackNewActiveTasks;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay)
	bool bShouldTaskUseWidgets;
};

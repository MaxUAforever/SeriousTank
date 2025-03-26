#pragma once

#include "QuestTaskInfoDataAsset.h"
#include "QuestCoreTypes.h"
#include "QuestTaskTimeLimitedInfoDataAsset.generated.h"

UCLASS(Abstract)
class QUESTSUBSYSTEM_API UQuestTaskTimeLimitedInfoDataAsset : public UQuestTaskInfoDataAsset
{
	GENERATED_BODY()

public:
	// Time limit for task completion in seconds.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings")
	float TimeLimit;

	// Result of the task when the time limit is reached.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings")
	EQuestCompleteRelust TimeOverCompleteResult;

	// Determines if task should be finished if it was interrupted by something (e.g. exiting game)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings")
	bool bShouldFinishOnInterrupt = true;
};

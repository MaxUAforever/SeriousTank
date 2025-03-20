#pragma once

#include "QuestTaskInfoDataAsset.h"
#include "QuestTaskTimeLimitedInfoDataAsset.generated.h"

enum class EQuestTaskCompleteResult : uint8;

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
	EQuestTaskCompleteResult TimeOverCompleteResult;
};

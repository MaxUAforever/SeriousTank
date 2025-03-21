#pragma once

#include "QuestSubsystem/Public/Data/Tasks/QuestTaskInfoDataAsset.h"
#include "ST_AIAreaClearingTaskInfoDataAsset.generated.h"

UCLASS()
class SERIOUSTANK_API UST_AIAreaClearingTaskInfoDataAsset : public UQuestTaskInfoDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Quest Settings")
	TArray<int32> EnemiesWavesAIAmount;
};

#pragma once

#include "QuestSubsystem/Public/Data/Tasks/QuestTaskTimeLimitedInfoDataAsset.h"
#include "ST_TargetShootingTaskInfoDataAsset.generated.h"

UCLASS()
class SERIOUSTANK_API UST_TargetShootingTaskInfoDataAsset : public UQuestTaskTimeLimitedInfoDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings", meta = (ClampMin = "1", ClampMax = "500"))
	float AddedTimeForTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings", meta = (ClampMin = "1", ClampMax = "500"))
	int32 AddedScoreForTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings", meta = (ClampMin = "0", ClampMax = "500"))
	int32 ScoreToWin;
};

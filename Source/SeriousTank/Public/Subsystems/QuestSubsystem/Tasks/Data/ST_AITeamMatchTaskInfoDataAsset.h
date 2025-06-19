#pragma once

#include "QuestSubsystem/Public/Data/Tasks/QuestTaskInfoDataAsset.h"
#include "ST_AITeamMatchTaskInfoDataAsset.generated.h"

UCLASS()
class SERIOUSTANK_API UST_AITeamMatchTaskInfoDataAsset : public UQuestTaskInfoDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Quest Settings")
	int32 RoundsToWin;

	UPROPERTY(EditDefaultsOnly, Category = "Quest Settings")
	TArray<uint8> TeamIDs;

	UPROPERTY(EditDefaultsOnly, Category = "Quest Settings")
	int32 DelayBetweenRounds;
};

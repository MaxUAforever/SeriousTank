#pragma once

#include "QuestSubsystem/Public/Tasks/TimeLimitedQuestTask.h"
#include "ST_QuestTask_TargetShooting.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTargetShotingScoreChangedDelegate, int32)

UCLASS()
class SERIOUSTANK_API UST_QuestTask_TargetShooting : public UTimeLimitedQuestTask
{
    GENERATED_BODY()

public:
	FTargetShotingScoreChangedDelegate OnScoreChangedDelegate;

public:
    virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) override;

	virtual void OnTaskStarted() override;

	int32 GetScore() const { return CurrentScore; }

private:
	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);
	void OnTargetSpawned(AActor* InTargetActor);

private:
	float AddedTimeForTarget;
	int32 AddedScoreForTarget;
	int32 ScoreToWin;

	int32 CurrentScore;
};

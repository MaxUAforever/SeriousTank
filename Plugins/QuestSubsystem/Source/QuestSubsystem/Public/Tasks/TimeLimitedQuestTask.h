#pragma once

#include "BaseQuestTask.h"
#include "Engine/TimerHandle.h"
#include "TimeLimitedQuestTask.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTaskRemainingTimeUpdated, float);

UCLASS()
class QUESTSUBSYSTEM_API UTimeLimitedQuestTask : public UBaseQuestTask
{
	GENERATED_BODY()

public:
	FOnTaskRemainingTimeUpdated OnTaskRemainingTimeUpdated;

public:
	float GetRemainingTime() const;

	void AddRemainingTime(float TimeToAdd);

protected:
	virtual void OnTaskStarted() override;
	virtual void OnTaskCompleted(EQuestTaskCompleteResult CompleteResult) override;

	virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) override;

private:
	void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	void OnTaskTimeLimitReached();

private:
	FTimerHandle TaskLimitTimerHandle;

	EQuestTaskCompleteResult TimeOverCompleteResult;
	float TaskLimitTime = 0.0f;
};

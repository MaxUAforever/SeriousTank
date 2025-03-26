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

	UFUNCTION()
	void SetSavedRemainingTime(float InRemainingTimeTime) { SavedRemainingTime = InRemainingTimeTime; }

	UFUNCTION()
	void SetSavedPassedTime(float InPassedTime) { SavedPassedTime = InPassedTime; }

protected:
	virtual void OnTaskStarted() override;
	virtual void OnTaskCompleted(EQuestCompleteRelust CompleteResult) override;

	virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) override;
	virtual void PreSaveGame() override;

private:
	void OnWorldCleanup(UWorld* World);

	void OnTaskTimeLimitReached();
	
private:
	FTimerHandle TaskLimitTimerHandle;

	float TaskLimitTime = 0.0f;
	bool bShouldFinishOnInterrupt = true;
	EQuestCompleteRelust TimeOverCompleteResult;
	
	float StartTime;

	UPROPERTY(SaveGame)
	float SavedRemainingTime = 0.0f;

	UPROPERTY(SaveGame)
	float SavedPassedTime = 0.0f;
};

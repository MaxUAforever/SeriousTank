#pragma once

#include "QuestSubsystem/Public/Tasks/BaseQuestTask.h"
#include "Engine/TimerHandle.h"
#include "ST_QuestTask_AITeamMatch.generated.h"

class ABaseObjectSpawner;
class UST_AITeamsManagerSubsystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundStartedDelegate, int32 /*RoundNumber*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundFinishedDelegate, int32 /*RoundNumber*/);

UCLASS()
class SERIOUSTANK_API UST_QuestTask_AITeamMatch : public UBaseQuestTask
{
    GENERATED_BODY()
 
public:
    FOnRoundStartedDelegate OnRoundStartedDelegate;
    FOnRoundFinishedDelegate OnRoundFinishedDelegate;

public:
    virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) override;

    int32 GetCurrentRound() const { return CurrentRound; }

protected:
	virtual void OnTaskStarted() override;
    virtual void BeginDestroy() override;
private:
    void StartNextRound();

    void OnTeamMemberDestroyed(uint8 TeamId, const AActor* DestroyedActor);
    void OnNextRoundDelayFinished();

private:
    int32 RoundsToWin;
    int32 DelayBetweenRounds;

    int32 CurrentRound;
    TMap<uint8, int32> TeamScores;

private:
    TObjectPtr<UST_AITeamsManagerSubsystem> CachedAITeamsManagerSubsystem;

    FTimerHandle FinishRoundDelayTimer;
};

#pragma once

#include "QuestSubsystem/Public/Tasks/BaseQuestTask.h"
#include "ST_QuestTask_AITeamMatch.generated.h"

class ABaseObjectSpawner;
class UST_AITeamsManagerSubsystem;

UCLASS()
class SERIOUSTANK_API UST_QuestTask_AITeamMatch : public UBaseQuestTask
{
    GENERATED_BODY()
 
public:
    virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) override;

    int32 GetCurrentRound() const { return CurrentRound; }

protected:
	virtual void OnTaskStarted() override;
    virtual void OnTaskCompleted(EQuestCompleteRelust CompleteResult) override;

private:
    void StartNextRound();

    void OnTeamMemberDestroyed(uint8 TeamId, const AActor* DestroyedActor);

private:
    int32 RoundsToWin;

    int32 CurrentRound;
    TMap<uint8, int32> TeamScores;

    TObjectPtr<UST_AITeamsManagerSubsystem> CachedAITeamsManagerSubsystem;
};

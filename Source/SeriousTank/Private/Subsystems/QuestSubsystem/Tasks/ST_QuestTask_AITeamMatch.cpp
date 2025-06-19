#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_AITeamMatch.h"

#include "Engine/World.h"
#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"
#include "Subsystems/QuestSubsystem/Tasks/Data/ST_AITeamMatchTaskInfoDataAsset.h"
#include "TimerManager.h"

void UST_QuestTask_AITeamMatch::FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset)
{
	const UST_AITeamMatchTaskInfoDataAsset* AreaClearingDataAsset = Cast<UST_AITeamMatchTaskInfoDataAsset>(QuestInfoDataAsset);
	if (!IsValid(AreaClearingDataAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AIAreaClearing::FillQuestInfo: AIAreaClearingTaskInfoDataAsset is not valid!"));
		return;
	}

	RoundsToWin = AreaClearingDataAsset->RoundsToWin;
	DelayBetweenRounds = AreaClearingDataAsset->DelayBetweenRounds;

	for (uint8 TeamID : AreaClearingDataAsset->TeamIDs)
	{
		TeamScores.Add({ TeamID, 0 });
	}
}

void UST_QuestTask_AITeamMatch::OnTaskStarted()
{
	CurrentRound = 0;
	for (auto& [TeamID, TeamScore] : TeamScores)
	{
		TeamScore = 0;
	}

	CachedAITeamsManagerSubsystem = GetWorld()->GetSubsystem<UST_AITeamsManagerSubsystem>();
	if (!IsValid(CachedAITeamsManagerSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AITeamMatch::StartNextRound: AITeamsManagerSubsystem is nullptr!"));
		return;
	}

	CachedAITeamsManagerSubsystem->OnTeamMemberDestroyedDelegate.AddUObject(this, &ThisClass::OnTeamMemberDestroyed);

	StartNextRound();
}

void UST_QuestTask_AITeamMatch::BeginDestroy()
{
	if (IsValid(CachedAITeamsManagerSubsystem))
	{
		CachedAITeamsManagerSubsystem->OnTeamMemberDestroyedDelegate.RemoveAll(this);
	}

	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FinishRoundDelayTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(FinishRoundDelayTimer);
	}

	Super::BeginDestroy();
}

void UST_QuestTask_AITeamMatch::StartNextRound()
{
	if (!IsValid(CachedAITeamsManagerSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AITeamMatch::StartNextRound: AITeamsManagerSubsystem is nullptr!"));
		return;
	}

	OnRoundStartedDelegate.Broadcast(CurrentRound);
	CachedAITeamsManagerSubsystem->RespawnAllTeamsMembers();
}

void UST_QuestTask_AITeamMatch::OnTeamMemberDestroyed(uint8 TeamId, const AActor* DestroyedActor)
{
	if (!IsValid(CachedAITeamsManagerSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_QuestTask_AITeamMatch::OnTeamMemberDestroyed: AITeamsManagerSubsystem is nullptr!"));
		return;
	}

	if (CachedAITeamsManagerSubsystem->GetActiveTeamMembersCount(TeamId) > 0)
	{
		return;
	}
	
	TeamScores[TeamId] += 1;
	OnRoundFinishedDelegate.Broadcast(CurrentRound);

	GetWorld()->GetTimerManager().SetTimer(FinishRoundDelayTimer, this, &ThisClass::OnNextRoundDelayFinished, DelayBetweenRounds, false);
}

void UST_QuestTask_AITeamMatch::OnNextRoundDelayFinished()
{
	for (auto& [TeamID, TeamScore] : TeamScores)
	{
		if (TeamScores[TeamID] >= RoundsToWin)
		{
			FinishTask(EQuestCompleteRelust::Succeeded);
			return;
		}
	}

	++CurrentRound;
	StartNextRound();
}

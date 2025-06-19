#include "GameFramework/Gameplay/Quests/ST_AITeamMatchGameMode.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"
#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_AITeamMatch.h"

void AST_AITeamMatchGameMode::OnQuestsStarted()
{
	UQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::OnQuestsStarted: QuestSubsystem is not valid"));
		return;
	}

	UST_QuestTask_AITeamMatch* AITeamMatchTask = Cast<UST_QuestTask_AITeamMatch>(QuestSubsystem->GetTrackedQuestTask());
	if (!IsValid(AITeamMatchTask))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::OnQuestsStarted: AITeamMatchTask is not valid"));
		return;
	}

	AITeamMatchTask->OnRoundStartedDelegate.AddUObject(this, &ThisClass::OnRoundStarted);
}

void AST_AITeamMatchGameMode::OnRoundStarted(int32 RoundNumber)
{
	if (RoundNumber <= 0)
	{
		return;
	}

	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
		GameplayGameState->SwitchToSpecificState(EInternalGameState::PreGameCountdown);
	}
}

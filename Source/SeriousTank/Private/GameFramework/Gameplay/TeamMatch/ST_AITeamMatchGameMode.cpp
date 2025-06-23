#include "GameFramework/Gameplay/TeamMatch/ST_AITeamMatchGameMode.h"

#include "Components/Teams/ST_TeamOwnershipComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/TeamMatch/ST_AITeamMatchPlayerState.h"
#include "GenericTeamAgentInterface.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"
#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"
#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"
#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_AITeamMatch.h"

void AST_AITeamMatchGameMode::OnPreStartCountdownStarted()
{
	Super::OnPreStartCountdownStarted();

	StartQuests();
}

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

	UST_HealthSubsystem* HealthSubsystem = GetWorld()->GetSubsystem<UST_HealthSubsystem>();
	if (!IsValid(HealthSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::OnQuestsStarted: HealthSubsystem is not valid"));
		return;
	}

	HealthSubsystem->OnActorEliminatedDelegate.AddUObject(this, &ThisClass::OnActorEliminated);
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

void AST_AITeamMatchGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);

	UST_AITeamsManagerSubsystem* AITeamsManagerSubsystem = GetWorld()->GetSubsystem<UST_AITeamsManagerSubsystem>();
	if (!IsValid(AITeamsManagerSubsystem) || !IsValid(NewPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::FinishRestartPlayer: Invalid AITeamsManagerSubsystem or NewPlayer"));
		return;
	}
	
	if (!IsValid(NewPlayer->GetPawn()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::FinishRestartPlayer: NewPlayer's Pawn is not valid"));
		return;
	}

	TWeakObjectPtr<AActor> SpawnPoint = NewPlayer->StartSpot;
	if (!SpawnPoint.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::FinishRestartPlayer: Invalid SpawnPoint for NewPlayer"));
		return;
	}

	UST_TeamOwnershipComponent* TeamOwnershipComponent = SpawnPoint->GetComponentByClass<UST_TeamOwnershipComponent>();
	if (!IsValid(TeamOwnershipComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::FinishRestartPlayer: Invalid TeamOwnershipComponent on SpawnPoint"));
		return;
	}

	const FGenericTeamId& TeamId = TeamOwnershipComponent->GetGenericTeamId();

	IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(NewPlayer);
	if (TeamAgentInterface != nullptr)
	{
		TeamAgentInterface->SetGenericTeamId(TeamId);
	}

	AITeamsManagerSubsystem->AddTeamMember(TeamId.GetId(), { NewPlayer->GetPawn(), NewPlayer, SpawnPoint.Get()});
}

void AST_AITeamMatchGameMode::OnActorEliminated(const AController* EliminatingPlayer, const AActor* EliminatedActor)
{
	const APawn* EliminatedPawn = Cast<APawn>(EliminatedActor);
	if (!IsValid(EliminatedPawn) || !IsValid(EliminatingPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::OnActorEliminated: Invalid EliminatedPawn or EliminatingPlayer"));
		return;
	}

	const AController* EliminatedController = EliminatedPawn->GetController();
	if (!IsValid(EliminatedController))
	{
		return;
	}

	AST_AITeamMatchPlayerState* EliminatingPlayerState = EliminatingPlayer->GetPlayerState<AST_AITeamMatchPlayerState>();
	AST_AITeamMatchPlayerState* EliminatedPlayerState = EliminatedController->GetPlayerState<AST_AITeamMatchPlayerState>();
	if (!IsValid(EliminatingPlayerState) || !IsValid(EliminatedPlayerState))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::OnActorEliminated: Invalid PlayerStates"));
		return;
	}

	const IGenericTeamAgentInterface* EliminatedTeamAgent = Cast<IGenericTeamAgentInterface>(EliminatedController);
	const IGenericTeamAgentInterface* EliminatingTeamAgent = Cast<IGenericTeamAgentInterface>(EliminatingPlayer);

	if (EliminatedTeamAgent == nullptr || EliminatingTeamAgent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_AITeamMatchGameMode::OnActorEliminated: Invalid Team Agent Interfaces"));
		return;
	}

	if (EliminatedTeamAgent->GetGenericTeamId() != EliminatingTeamAgent->GetGenericTeamId())
	{
		EliminatingPlayerState->AddKillNum(1);
	}
	
	EliminatedPlayerState->AddDeathNum(1);
}

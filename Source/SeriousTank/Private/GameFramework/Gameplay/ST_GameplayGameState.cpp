#include "GameFramework/Gameplay/ST_GameplayGameState.h"

#include "Core/ST_CoreTypes.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "ObjectPoolSubsystem/Public/ObjectPoolSubsystem.h"
#include "PlayerInteractionSubsystem/Public/PlayerInteractionSubsystem.h"
#include "TimerManager.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"

DEFINE_LOG_CATEGORY(GameStateLog);

AST_GameplayGameState::AST_GameplayGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InternalGameState = EInternalGameState::WaitingToInitialize;
}

void AST_GameplayGameState::Initialize(const FBaseGameData& InGameData)
{
	if (InternalGameState != EInternalGameState::WaitingToInitialize)
	{
		return;
	}

	bIsUsingQuestSubsystem = InGameData.bIsUsingQuestSubsystem;
	PreStartCountdownTime = InGameData.PreStartCountdownTime;

	OnStateWaitingToInitializeStarted();
}

float AST_GameplayGameState::GetPreStartCountdownTime() const
{
	if (InternalGameState != EInternalGameState::PreGameCountdown)
	{
		return 0.f;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return 0.f;
	}

	return World->GetTimerManager().GetTimerRemaining(PreStartCountdownTimer);
}

void AST_GameplayGameState::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(PreStartCountdownTimer);
	}
}

void AST_GameplayGameState::SwitchToNextState()
{
	switch (InternalGameState)
	{
		case EInternalGameState::WaitingToInitialize:
		{
			InternalGameState = EInternalGameState::PreGameCountdown;
			OnStatePreGameCountdownStarted();
			break;
		}
		case EInternalGameState::PreGameCountdown:
		{
			InternalGameState = EInternalGameState::GameInProgress;
			OnStateGameInProgressStarted();
			break;
		}
		case EInternalGameState::GameInProgress:
		{
			InternalGameState = EInternalGameState::FinalGameResults;
			OnStateFinalGameResultsStarted();
			break;
		}
		case EInternalGameState::FinalGameResults:
		{
			InternalGameState = EInternalGameState::LeavingGame;
			OnStateLeavingGameStarted();
			break;
		}
		case EInternalGameState::LeavingGame:
		{
			break;
		}
		default:
		{
			UE_LOG(LogTemp, Error, TEXT("AST_GameplayGameState::SwitchToNextState: unexpected internal game state"));
			break;
		}
	}

	OnInternalGameStateChangedDelegate.Broadcast(InternalGameState);
}

void AST_GameplayGameState::SwitchToSpecificState(EInternalGameState NewState)
{
	TMap<EInternalGameState, EInternalGameState> PreviousStateMap = {
		{EInternalGameState::PreGameCountdown, EInternalGameState::WaitingToInitialize},
		{EInternalGameState::GameInProgress, EInternalGameState::PreGameCountdown},
		{EInternalGameState::FinalGameResults, EInternalGameState::GameInProgress},
		{EInternalGameState::LeavingGame, EInternalGameState::FinalGameResults}
	};

	if (NewState == InternalGameState || !PreviousStateMap.Contains(NewState))
	{
		return;
	}

	InternalGameState = *PreviousStateMap.Find(NewState);
	SwitchToNextState();
}

void AST_GameplayGameState::OnStateWaitingToInitializeStarted()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!IsValid(GameInstance))
	{
		return;
	}

	// Waiting for initialization of Quest Subsystem
	UQuestSubsystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>();
	if (bIsUsingQuestSubsystem && IsValid(QuestSubsystem) && QuestSubsystem->GetAllQuests().IsEmpty())
	{
		static FDelegateHandle QuestInitHandle;
		auto OnQuestSubsystemInitialized = [this, QuestSubsystem]()
		{
			QuestSubsystem->GetOnQuestsInitializedDelegate().Remove(QuestInitHandle);
			OnSubsystemInitialized();
		};

		// QuestSubsystem can be initialized multiple times, so we are using separate lambda to prevent triggering more then once.
		QuestInitHandle = QuestSubsystem->GetOnQuestsInitializedDelegate().AddLambda(OnQuestSubsystemInitialized);
		++SubsystemsToInitialize;
	}

	// Waiting for initialization of Player Interaction Subsystem
	UPlayerInteractionSubsystem* PlayerInteractionSubsystem = World->GetSubsystem<UPlayerInteractionSubsystem>();
	if (IsValid(PlayerInteractionSubsystem) && !PlayerInteractionSubsystem->IsInitialized())
	{
		PlayerInteractionSubsystem->GetOnSubsystemInitializedDelegate().AddUObject(this, &ThisClass::OnSubsystemInitialized);
		++SubsystemsToInitialize;
	}

	// Waiting for initialization of Object Pool Subsystem
	UObjectPoolSubsystem* ObjectPoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
	if (IsValid(ObjectPoolSubsystem) && !ObjectPoolSubsystem->IsInitialized())
	{
		ObjectPoolSubsystem->GetOnObjectPoolSubsystemInitializedDelegate().AddUObject(this, &ThisClass::OnSubsystemInitialized);
		++SubsystemsToInitialize;
	}

	if (SubsystemsToInitialize <= 0)
	{
		OnSubsystemInitialized();
	}
}

void AST_GameplayGameState::OnStatePreGameCountdownStarted()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (PreStartCountdownTime > 0.f)
	{
		World->GetTimerManager().SetTimer(PreStartCountdownTimer, this, &ThisClass::OnPreGameCountdownFinished, PreStartCountdownTime, false);
		
		OnPreStartCountdownStartedDelegate.Broadcast();
	}
	else
	{
		OnPreGameCountdownFinished();
	}
}

void AST_GameplayGameState::OnStateGameInProgressStarted()
{
	SetActorTickEnabled(true);
}

void AST_GameplayGameState::OnStateFinalGameResultsStarted()
{
	SetActorTickEnabled(false);
	OnGameIsOver.Broadcast();
}

void AST_GameplayGameState::OnStateLeavingGameStarted()
{
	// TODO: Add loading screen logic.
}

void AST_GameplayGameState::OnSubsystemInitialized()
{
	if (--SubsystemsToInitialize <= 0)
	{
		OnGameStateInitializedDelegate.Broadcast();
		SwitchToNextState();
	}
}

void AST_GameplayGameState::OnPreGameCountdownFinished()
{
	OnPreStartCountdownEndedDelegate.Broadcast();
	SetActorTickEnabled(true);

	SwitchToNextState();
}

void AST_GameplayGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TotalPlayTime += DeltaSeconds;
}

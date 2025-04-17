#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_GameplayGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Display, All);

UENUM()
enum class EInternalGameState : uint8
{
	WaitingToInitialize,
	PreGameCountdown,
	GameInProgress,
	FinalGameResults,
	LeavingGame
};

struct FBaseGameData;

DECLARE_MULTICAST_DELEGATE(FOnGameStateInitializedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGameIsOver);
DECLARE_DELEGATE_OneParam(FCountdownChanged, int32);
DECLARE_MULTICAST_DELEGATE(FOnCountdownEnded);

UCLASS()
class SERIOUSTANK_API AST_GameplayGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	FOnGameStateInitializedDelegate OnGameStateInitializedDelegate;
	FOnGameIsOver OnGameIsOver;
	FOnCountdownEnded OnPreStartCountdownEndedDelegate;

public:
	AST_GameplayGameState();

	void Initialize(const FBaseGameData& GameData);

protected:
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void Tick(float DeltaSeconds) override;

public:
	EInternalGameState GetInternalGameState() const { return InternalGameState; }

	float GetPreStartCountdownTime() const;
	float GetTotalPlayTime() const { return TotalPlayTime; };
	
	void SwitchToNextState();

private:
	void OnStateWaitingToInitializeStarted();
	void OnStatePreGameCountdownStarted();
	void OnStateGameInProgressStarted();
	void OnStateFinalGameResultsStarted();
	void OnStateLeavingGameStarted();

	void OnSubsystemInitialized();
	void OnPreGameCountdownFinished();

private:
	bool bIsUsingQuestSubsystem = false;
	int32 SubsystemsToInitialize = 0;

	float TotalPlayTime = 0.f;
	int32 PreStartCountdownTime = 0;
	FTimerHandle PreStartCountdownTimer;

	EInternalGameState InternalGameState;
};

#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_GameplayGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Display, All);

DECLARE_MULTICAST_DELEGATE(FOnGameIsOver);
DECLARE_DELEGATE_OneParam(FCountdownChanged, int32);
DECLARE_MULTICAST_DELEGATE(FOnCountdownEnded);

UCLASS()
class SERIOUSTANK_API AST_GameplayGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	FOnGameIsOver OnGameIsOver;

	FCountdownChanged OnPreStartCountdownChanged;
	FOnCountdownEnded OnPreStartCountdownEndedDelegate;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 RemainingCountdownTime = 0;

	UPROPERTY(VisibleAnywhere)
	float TotalPlayTime = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Timers")
	FTimerHandle PreStartCountdownTimer;

public:
	AST_GameplayGameState();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	float GetTotalPlayTime() const;

	int32 GetPreStartCountdownTime() const;
	void SetPreStartCountdownTime(int32 NewTime);

private:
	UFUNCTION()
	void OnPreStartCountdownTimerFired();
};

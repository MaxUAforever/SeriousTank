#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_GameplayGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Display, All);

UCLASS()
class SERIOUSTANK_API AST_GameplayGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnGameIsOver)
	FOnGameIsOver OnGameIsOver;

	DECLARE_DELEGATE_OneParam(FCountdownChanged, int32)
	FCountdownChanged OnPreStartCountdownChanged;

	DECLARE_DELEGATE(FCountdownEnded)
	FCountdownEnded OnPreStartCountdownEnded;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 RemainingCountdownTime = 0;

	UPROPERTY(VisibleAnywhere)
	float TotalPlayTime = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Timers")
	FTimerHandle PreStartCountdownTimer;

public:
	AST_GameplayGameState();

	float GetTotalPlayTime() const;

	int32 GetPreStartCountdownTime() const;
	void SetPreStartCountdownTime(int32 NewTime);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnPreStartCountdownTimerFired();
};

#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_GameplayGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Display, All);

UCLASS()
class SERIOUSTANK_API AST_GameplayGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FCountdownChanged, int32)
	FCountdownChanged OnPreStartCountdownChanged;

	DECLARE_DELEGATE(FCountdownEnded)
	FCountdownEnded OnPreStartCountdownEnded;

	DECLARE_DELEGATE(FTimeHasEnded)
	FTimeHasEnded OnTimeHasEnded;

	DECLARE_DELEGATE_OneParam(FScoreHasChanged, int32)
	FScoreHasChanged OnScoreHasChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 RemainingCountdownTime = 0;

	UPROPERTY(VisibleAnywhere)
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere)
	float RemainingTime = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Timers")
	FTimerHandle PreStartCountdownTimer;

public:
	AST_GameplayGameState();

	int32 GetScore() const;
	void SetRemainingTime(float NewTime);
	void AddTime(float DeltaTime);

	float GetRemainingTime() const;
	void SetScore(int32 NewScore);
	void AddScore(int32 DeltaScore);

	int32 GetPreStartCountdownTime() const;
	void SetPreStartCountdownTime(int32 NewTime);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnPreStartCountdownTimerFired();
};

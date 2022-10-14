#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_GameplayGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Display, All);

UCLASS()
class SERIOUSTANK_API AST_GameplayGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FTimeHasEnded)
	FTimeHasEnded OnTimeHasEnded;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere)
	float RemainingTime = 0.f;

public:
	AST_GameplayGameState();

	int32 GetScore() const;
	void SetRemainingTime(float NewTime);
	void AddTime(float DeltaTime);

	float GetRemainingTime() const;
	void SetScore(int32 NewScore);
	void AddScore(int32 DeltaScore);

protected:
	virtual void Tick(float DeltaSeconds) override;
};

#pragma once

#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "ST_TargetShootingGameState.generated.h"

UCLASS()
class SERIOUSTANK_API AST_TargetShootingGameState : public AST_GameplayGameState
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_OneParam(FScoreHasChanged, int32)
	FScoreHasChanged OnScoreHasChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere)
	float RemainingTime = 0.f;

public:
	AST_TargetShootingGameState();

	float GetRemainingTime() const;
	void SetRemainingTime(float NewTime);
	void AddRemainingTime(float DeltaTime);

	int32 GetScore() const;
	void SetScore(int32 NewScore);
	void AddScore(int32 DeltaScore);

protected:
	virtual void Tick(float DeltaSeconds) override;
};

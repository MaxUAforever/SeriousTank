#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_GameplayGameState.generated.h"

UCLASS()
class SERIOUSTANK_API AST_GameplayGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere)
	float RemainingTime = 0.f;

public:
	int32 GetScore() const;
	void SetRemainingTime(float NewTime);
	void AddTime(float DeltaTime);

	float GetRemainingTime() const;
	void SetScore(int32 NewScore);
	void AddScore(int32 DeltaScore);
};

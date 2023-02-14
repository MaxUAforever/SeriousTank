#include "GameFramework/Gameplay/TargetShooting/ST_TargetShootingGameState.h"

AST_TargetShootingGameState::AST_TargetShootingGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

int32 AST_TargetShootingGameState::GetScore() const
{
	return Score;
}

void AST_TargetShootingGameState::SetScore(const int32 NewScore)
{
	Score = NewScore;
}

void AST_TargetShootingGameState::AddScore(const int32 DeltaScore)
{
	Score = FMath::Max(Score + DeltaScore, 0);

	OnScoreHasChanged.ExecuteIfBound(Score);
}

float AST_TargetShootingGameState::GetRemainingTime() const
{
	return RemainingTime;
}

void AST_TargetShootingGameState::SetRemainingTime(const float NewTime)
{
	RemainingTime = NewTime;
}

void AST_TargetShootingGameState::AddRemainingTime(const float DeltaTime)
{
	RemainingTime = FMath::Max(RemainingTime + DeltaTime, 0.f);
	if (FMath::IsNearlyZero(RemainingTime))
	{
		OnGameIsOver.Broadcast();
		SetActorTickEnabled(false);
	}
}

void AST_TargetShootingGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddRemainingTime(DeltaSeconds * -1);
}
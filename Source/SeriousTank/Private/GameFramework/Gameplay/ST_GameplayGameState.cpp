#include "GameFramework/Gameplay/ST_GameplayGameState.h"

DEFINE_LOG_CATEGORY(GameStateLog);

AST_GameplayGameState::AST_GameplayGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

int32 AST_GameplayGameState::GetScore() const
{
	return Score;
}

void AST_GameplayGameState::SetScore(const int32 NewScore)
{
	Score = NewScore;
}

void AST_GameplayGameState::AddScore(const int32 DeltaScore)
{
	Score = FMath::Max(Score + DeltaScore, 0);

	UE_LOG(GameStateLog, Warning, TEXT("Score: %d"), Score);
}

float AST_GameplayGameState::GetRemainingTime() const
{
	return RemainingTime;
}

void AST_GameplayGameState::SetRemainingTime(const float NewTime)
{
	RemainingTime = NewTime;
}

void AST_GameplayGameState::AddTime(const float DeltaTime)
{
	RemainingTime = FMath::Max(RemainingTime + DeltaTime, 0.f);
	if (FMath::IsNearlyZero(RemainingTime))
	{
		OnTimeHasEnded.ExecuteIfBound();
	}
}

void AST_GameplayGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddTime(DeltaSeconds * -1);
}
#include "GameFramework/Gameplay/ST_GameplayGameState.h"

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
	Score += DeltaScore;
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
	RemainingTime += DeltaTime;
}
#include "GameFramework/Gameplay/ST_GameplayGameState.h"

#include "Engine/World.h"
#include "TimerManager.h"

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

	OnScoreHasChanged.ExecuteIfBound(Score);
}

int32 AST_GameplayGameState::GetPreStartCountdownTime() const
{
	return RemainingCountdownTime;
}

void AST_GameplayGameState::SetPreStartCountdownTime(int32 NewTime)
{
	RemainingCountdownTime = NewTime;
}

float AST_GameplayGameState::GetRemainingTime() const
{
	return RemainingTime;
}

void AST_GameplayGameState::SetRemainingTime(const float NewTime)
{
	RemainingTime = NewTime;
}

void AST_GameplayGameState::AddRemainingTime(const float DeltaTime)
{
	RemainingTime = FMath::Max(RemainingTime + DeltaTime, 0.f);
	if (FMath::IsNearlyZero(RemainingTime))
	{
		OnTimeHasEnded.Broadcast();
	}
}

float AST_GameplayGameState::GetTotalPlayTime() const
{
	return TotalPlayTime;
}

void AST_GameplayGameState::BeginPlay()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	FTimerDelegate PreStartCountdownTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnPreStartCountdownTimerFired);
	World->GetTimerManager().SetTimer(PreStartCountdownTimer, PreStartCountdownTimerDelegate, 1.f, true);
}

void AST_GameplayGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddRemainingTime(DeltaSeconds * -1);
	TotalPlayTime += DeltaSeconds;
}

void AST_GameplayGameState::OnPreStartCountdownTimerFired()
{
	OnPreStartCountdownChanged.ExecuteIfBound(--RemainingCountdownTime);

	if (RemainingCountdownTime == 0)
	{
		OnPreStartCountdownEnded.ExecuteIfBound();

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(PreStartCountdownTimer);
			SetActorTickEnabled(true);
		}
	}
}

#include "GameFramework/Gameplay/ST_GameplayGameState.h"

#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(GameStateLog);

AST_GameplayGameState::AST_GameplayGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

int32 AST_GameplayGameState::GetPreStartCountdownTime() const
{
	return RemainingCountdownTime;
}

void AST_GameplayGameState::SetPreStartCountdownTime(int32 NewTime)
{
	RemainingCountdownTime = NewTime;
}

float AST_GameplayGameState::GetTotalPlayTime() const
{
	return TotalPlayTime;
}

void AST_GameplayGameState::BeginPlay()
{
	Super::BeginPlay();

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

	TotalPlayTime += DeltaSeconds;
}

void AST_GameplayGameState::OnPreStartCountdownTimerFired()
{
	OnPreStartCountdownChanged.ExecuteIfBound(--RemainingCountdownTime);

	if (RemainingCountdownTime == 0)
	{
		OnPreStartCountdownEndedDelegate.Broadcast();

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(PreStartCountdownTimer);
			SetActorTickEnabled(true);
		}
	}
}

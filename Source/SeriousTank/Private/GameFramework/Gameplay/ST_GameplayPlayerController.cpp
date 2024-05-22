#include "GameFramework/Gameplay/ST_GameplayPlayerController.h"

#include "GameFramework/Gameplay/ST_GameplayGameMode.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"

#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerInput.h"
#include "InputAction.h"

void AST_GameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetPawnInputEnabled(false);
	SetInputMode(FInputModeGameOnly{});

	if (UWorld* World = GetWorld())
	{
		if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
		{
			GameState->OnPreStartCountdownEnded.BindUObject(this, &ThisClass::SetPawnInputEnabled, true);
			GameState->OnGameIsOver.AddUObject(this, &ThisClass::SetOnlyUIInputEnabled, true);
		}
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CommonGameplayInputContext && !EnhancedSubsystem->HasMappingContext(CommonGameplayInputContext))
		{
			EnhancedSubsystem->AddMappingContext(CommonGameplayInputContext, 0);
		}
	}
}

void AST_GameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Started, this, &ThisClass::OnPauseGameClicked);
		}
	}

	//InputComponent->BindAction("PauseGame", EInputEvent::IE_Pressed, this, &ThisClass::OnPauseGameClicked).bExecuteWhenPaused = true;
}

void AST_GameplayPlayerController::OnUnPossess()
{
	PreviousPawn = GetPawn();

	Super::OnUnPossess();
}

void AST_GameplayPlayerController::SetPawnInputEnabled(bool IsEnabled)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	if (IsEnabled)
	{
		ControlledPawn->EnableInput(this);
	}
	else
	{
		ControlledPawn->DisableInput(this);
	}
}

void AST_GameplayPlayerController::SetOnlyUIInputEnabled(bool IsEnabled)
{
	SetPawnInputEnabled(!IsEnabled);

	bShowMouseCursor = IsEnabled;
	bEnableClickEvents = IsEnabled;

	if (IsEnabled)
	{
		SetInputMode(FInputModeUIOnly{});
	}
	else
	{
		SetInputMode(FInputModeGameOnly{});
	}
}

void AST_GameplayPlayerController::OnPauseGameClicked()
{
	SetPause(!IsPaused());

	SetOnlyUIInputEnabled(IsPaused());
}

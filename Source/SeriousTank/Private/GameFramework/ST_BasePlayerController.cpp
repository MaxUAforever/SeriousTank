#include "GameFramework/ST_BasePlayerController.h"

#include "Engine/GameViewportClient.h"
#include "Engine/World.h"

void AST_BasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("UINavigation", EInputEvent::IE_Pressed, this, &ThisClass::HandleNavigationKeyPressed).bExecuteWhenPaused = true;
}

void AST_BasePlayerController::HandleNavigationKeyPressed()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UGameViewportClient* GameViewportClient = World->GetGameViewport();
	if (!GameViewportClient)
	{
		return;
	}

	bool bIsInputIgnored = GameViewportClient->IgnoreInput();
	EMouseCaptureMode MouseCaptureMode = GameViewportClient->GetMouseCaptureMode();

	bool bIsGameAndUIInput = bIsInputIgnored == false && MouseCaptureMode == EMouseCaptureMode::CaptureDuringMouseDown;
	bool bIsUIInput = bIsInputIgnored == true && MouseCaptureMode == EMouseCaptureMode::NoCapture;

	if (bIsGameAndUIInput || bIsUIInput)
	{
		OnNavigationKeyPressed.Broadcast();
	}
}
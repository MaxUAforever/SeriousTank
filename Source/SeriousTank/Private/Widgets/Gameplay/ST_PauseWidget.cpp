#include "Widgets/Gameplay/ST_PauseWidget.h"

#include "GameFramework/Gameplay/ST_GameplayPlayerController.h"
#include "GameFramework/ST_GameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UST_PauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &ThisClass::OnResumeButtonClicked);
	MenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMenuButtonClicked);
}

void UST_PauseWidget::OnResumeButtonClicked()
{
	if (UWorld* World = GetWorld())
	{
		AST_GameplayPlayerController* PlayerController = Cast<AST_GameplayPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
		if (PlayerController)
		{
			PlayerController->OnPauseGameClicked();
		}
	}
}

void UST_PauseWidget::OnMenuButtonClicked()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
		{
			PlayerController->ClientTravel(GameInstance->GetMainMenuLevelName(), ETravelType::TRAVEL_Relative);
		}
	}
}

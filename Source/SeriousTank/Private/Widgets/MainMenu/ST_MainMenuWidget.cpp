#include "Widgets/MainMenu/ST_MainMenuWidget.h"

#include "GameFramework/MainMenu/ST_MainMenuPlayerController.h"
#include "GameFramework/ST_GameInstance.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UST_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGameButtonClicked);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		const FGameplayLevelInfo* DefaultLevelInfo = GameInstance->GetLevelInfo(0);
		if (DefaultLevelInfo)
		{
			ChooseLevelButtonCaption->SetText(FText::FromString(DefaultLevelInfo->LevelName));
		}
	}
}

void UST_MainMenuWidget::OnStartGameButtonClicked()
{
	if (UWorld* World = GetWorld())
	{
		if (AST_MainMenuPlayerController* PlayerController = Cast<AST_MainMenuPlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			PlayerController->OnNewGameStarted();
		}
	}
}

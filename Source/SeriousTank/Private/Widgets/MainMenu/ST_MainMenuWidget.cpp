#include "Widgets/MainMenu/ST_MainMenuWidget.h"

#include "GameFramework/MainMenu/ST_MainMenuPlayerController.h"
#include "GameFramework/ST_GameInstance.h"
#include "Widgets/MainMenu/ST_GameplayLevelCardWidget.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UST_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGameButtonClicked);
	ChooseGameplayLevelButton->OnClicked.AddDynamic(this, &ThisClass::OnChooseLevelButtonClicked);
	ExitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	SetCurrentLevelCaption(0);

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		int32 LevelIndex = 0;
		for (const FGameplayLevelInfo& LevelInfo : GameInstance->GetLevelsInfo())
		{
			AddLevelCardWidget(LevelInfo, LevelIndex++);
		}
	}

	if (BackgroundMusic)
	{
		UGameplayStatics::PlaySound2D(World, BackgroundMusic);
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

void UST_MainMenuWidget::OnChooseLevelButtonClicked()
{
	LevelsScrollBox->SetVisibility(LevelsScrollBox->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UST_MainMenuWidget::OnLevelIsChoosen(int32 LevelIndex)
{
	SetCurrentLevelCaption(LevelIndex);
	LevelsScrollBox->SetVisibility(ESlateVisibility::Hidden);
}

void UST_MainMenuWidget::OnExitButtonClicked()
{
	FGenericPlatformMisc::RequestExit(false);
}

void UST_MainMenuWidget::AddLevelCardWidget(const FGameplayLevelInfo& LevelInfo, const int32 LevelIndex)
{
	UST_GameplayLevelCardWidget* LevelCardInfo = CreateWidget<UST_GameplayLevelCardWidget>(GetWorld(), LevelCardWidgetClass);
	LevelCardInfo->SetPadding(FMargin{10.f, 10.f});
	LevelCardInfo->SetLevelName(FText::FromString(LevelInfo.LevelCaption));
	LevelCardInfo->SetLevelIndex(LevelIndex);

	LevelCardInfo->OnLevelButtonClicked.AddUObject(this, &ThisClass::OnLevelIsChoosen);

	LevelsScrollBox->AddChild(LevelCardInfo);
}

void UST_MainMenuWidget::SetCurrentLevelCaption(int32 LevelIndex)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		const FGameplayLevelInfo* DefaultLevelInfo = GameInstance->GetLevelInfo(LevelIndex);
		if (DefaultLevelInfo)
		{
			ChooseLevelButtonCaption->SetText(FText::FromString(DefaultLevelInfo->LevelCaption));
		}
	}
}

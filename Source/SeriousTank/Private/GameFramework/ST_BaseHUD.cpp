#include "GameFramework/ST_BaseHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AST_BaseHUD::BeginPlay()
{
	Super::BeginPlay();

	GameUIWidget = CreateAndAddWidget(GameUIClass);
	SettingsWidget = CreateAndAddWidget(SettingsWidgetClass);

	if (GameUIWidget)
	{
		GameUIWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget = GameUIWidget;
	}
}

void AST_BaseHUD::SwitchToMainWidget()
{
    SwitchToWidget(GameUIWidget);
}

void AST_BaseHUD::SwitchToSettingsWidget()
{
    SwitchToWidget(SettingsWidget);
}

UUserWidget* AST_BaseHUD::CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UUserWidget* UIWidget = CreateWidget<UUserWidget>(World, WidgetClass);
	if (UIWidget)
	{
		UIWidget->AddToViewport();
		UIWidget->SetVisibility(ESlateVisibility::Hidden);

		return UIWidget;
	}

	return nullptr;
}

void AST_BaseHUD::SwitchToWidget(UUserWidget* NeededWidget)
{
    if (NeededWidget == nullptr || NeededWidget == CurrentWidget)
    {
        return;
    }
    
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    NeededWidget->SetVisibility(ESlateVisibility::Visible);
    CurrentWidget = NeededWidget;
}

#include "GameFramework/ST_BaseHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AST_BaseHUD::BeginPlay()
{
	Super::BeginPlay();

	GameUIWidget = CreateAndAddWidget(GameUIClass);
	if (GameUIWidget)
	{
		GameUIWidget->SetVisibility(ESlateVisibility::Visible);
	}
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

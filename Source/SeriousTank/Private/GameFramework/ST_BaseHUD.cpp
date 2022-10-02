#include "GameFramework/ST_BaseHUD.h"

#include "Blueprint/UserWidget.h"

void AST_BaseHUD::BeginPlay()
{
	Super::BeginPlay();

	GameUIWidget = CreateWidget<UUserWidget>(GetWorld(), GameUIClass);
	if (GameUIWidget)
	{
		GameUIWidget->AddToViewport();
	}
}

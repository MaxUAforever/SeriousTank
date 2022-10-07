#include "GameFramework/ST_BaseHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AST_BaseHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = Cast<APlayerController>(GetOwner());
	GameUIWidget = CreateWidget<UUserWidget>(Controller, GameUIClass);
	if (GameUIWidget)
	{
		GameUIWidget->AddToViewport();
	}
}

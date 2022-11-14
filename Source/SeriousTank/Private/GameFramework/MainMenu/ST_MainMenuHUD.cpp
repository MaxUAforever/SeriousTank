#include "GameFramework/MainMenu/ST_MainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AST_MainMenuHUD::SetSettingsWidgetVisible(bool bShowSettings)
{
	if (GameUIWidget && SettingsWidget)
	{
		GameUIWidget->SetVisibility(bShowSettings ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		SettingsWidget->SetVisibility(bShowSettings ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
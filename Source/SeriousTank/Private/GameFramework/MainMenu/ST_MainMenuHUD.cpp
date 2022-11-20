#include "GameFramework/MainMenu/ST_MainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AST_MainMenuHUD::BeginPlay()
{
    Super::BeginPlay();
    
    GarageWidget = CreateAndAddWidget(GarageWidgetClass);
}

void AST_MainMenuHUD::SwitchToGarageWidget()
{
    SwitchToWidget(GarageWidget);
}

void AST_MainMenuHUD::SwitchFromSettingsWidget()
{
    SwitchToMainWidget();
}

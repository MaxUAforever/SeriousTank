#include "Widgets/MainMenu/VehicleChoosingMenu/ST_VehicleChoosingWidget.h"

#include "GameFramework/ST_BaseHUD.h"

void UST_VehicleChoosingWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    AcceptButton->OnClicked.AddDynamic(this, &ThisClass::OnAcceptButtonClicked);
}

void UST_VehicleChoosingWidget::OnAcceptButtonClicked()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    if (AST_MainMenuPlayerController* PlayerController = World->GetFirstPlayerController<AST_MainMenuPlayerController>())
    {
        if (AST_BaseHUD* BaseHUD = PlayerController->GetHUD<AST_BaseHUD>())
        {
            BaseHUD->SwitchToMainWidget();
        }
    }
}




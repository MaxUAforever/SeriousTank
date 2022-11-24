#include "Widgets/MainMenu/VehicleChoosingMenu/ST_VehicleChoosingWidget.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "GameFramework/ST_BaseHUD.h"
#include "Widgets/MainMenu/VehicleChoosingMenu/ST_VehicleInfoCardWidget.h"

void UST_VehicleChoosingWidget::NativeConstruct()
{
    Super::NativeConstruct();
    AcceptButton->OnClicked.AddDynamic(this, &ThisClass::OnAcceptButtonClicked);
    
    FillVehicleScrollBox();
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

void UST_VehicleChoosingWidget::FillVehicleScrollBox()
{
    UWorld* World = GetWorld();
    if (!World || !VehicleInfoCardClass)
    {
        return;
    }
    
    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC)
    {
        return;
    }
    
    AST_MainMenuPlayerState* PlayerState = PC->GetPlayerState<AST_MainMenuPlayerState>();
    if (!PlayerState)
    {
        return;
    }
    
    int32 VehicleIndex = 0;
    for (FVehicleInfo VehicleInfo : PlayerState->GetVehicles())
    {
        AST_BaseVehicle* DefaultBaseVehicle = VehicleInfo.VehicleClass->GetDefaultObject<AST_BaseVehicle>();
        if (!DefaultBaseVehicle)
        {
            continue;
        }
        
        UST_VehicleInfoCardWidget* VehicleCardInfo = CreateWidget<UST_VehicleInfoCardWidget>(World, VehicleInfoCardClass);
        
        VehicleCardInfo->SetPadding(FMargin{5.f, 5.f});
        VehicleCardInfo->SetDisplayName(DefaultBaseVehicle->GetDisplayName());
        VehicleCardInfo->SetVehicleIndex(VehicleIndex++);

        VehiclesScrollBox->AddChild(VehicleCardInfo);
    }
}


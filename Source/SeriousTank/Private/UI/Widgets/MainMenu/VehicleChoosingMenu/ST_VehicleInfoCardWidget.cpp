#include "UI/Widgets/MainMenu/VehicleChoosingMenu/ST_VehicleInfoCardWidget.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "GameFramework/ST_GameInstance.h"

#include "GameFramework/PlayerController.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UST_VehicleInfoCardWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    VehicleButton->OnClicked.AddDynamic(this, &ThisClass::OnVehicleButtonClicked);
}

void UST_VehicleInfoCardWidget::SetDisplayName(FString Name)
{
    VehicleNameBlock->SetText(FText::FromString(Name));
}

void UST_VehicleInfoCardWidget::SetVehicleIndex(int32 NewVehicleIndex)
{
    VehicleIndex = NewVehicleIndex;
}

void UST_VehicleInfoCardWidget::OnVehicleButtonClicked()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    if (APlayerController* PlayerController = World->GetFirstPlayerController())
    {
        if (AST_MainMenuPlayerState* PlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
        {
            PlayerState->SetCurrentVehicle(VehicleIndex);
        }
    }
}



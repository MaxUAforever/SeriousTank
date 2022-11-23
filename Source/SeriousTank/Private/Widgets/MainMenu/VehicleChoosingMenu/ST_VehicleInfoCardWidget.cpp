#include "Widgets/MainMenu/VehicleChoosingMenu/ST_VehicleInfoCardWidget.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "GameFramework/ST_GameInstance.h"

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

void UST_VehicleInfoCardWidget::SetVehicleClass(TSubclassOf<AST_BaseVehicle> NewVehicleClass)
{
    VehicleClass = NewVehicleClass;
}

void UST_VehicleInfoCardWidget::OnVehicleButtonClicked()
{
    if (UST_GameInstance* GameInstance = GetGameInstance<UST_GameInstance>())
    {
        GameInstance->SetVehicle(VehicleClass);
    }
}



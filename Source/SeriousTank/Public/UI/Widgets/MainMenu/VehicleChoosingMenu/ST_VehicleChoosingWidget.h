#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_VehicleChoosingWidget.generated.h"

class UButton;
class UScrollBox;
class UST_VehicleInfoCardWidget;

UCLASS()
class SERIOUSTANK_API UST_VehicleChoosingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
    UButton* AcceptButton;
	
    UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
    UButton* CustomiseButton;
    
    UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
    UScrollBox* VehiclesScrollBox;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UST_VehicleInfoCardWidget> VehicleInfoCardClass;
    
public:
    virtual void NativeConstruct() override;
    
private:
    UFUNCTION()
    void OnAcceptButtonClicked();
    
    void FillVehicleScrollBox();
};

#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_VehicleInfoCardWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SERIOUSTANK_API UST_VehicleInfoCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* VehicleNameBlock;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UButton* VehicleButton;
    
private:
    int32 VehicleIndex;
    
public:
    void SetDisplayName(FString Name);
    void SetVehicleIndex(int32 NewVehicleIndex);
    
protected:
    virtual void NativeConstruct() override;
    
private:
    UFUNCTION()
    void OnVehicleButtonClicked();
};

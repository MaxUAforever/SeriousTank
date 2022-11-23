#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_VehicleInfoCardWidget.generated.h"

class AST_BaseVehicle;
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
    TSubclassOf<AST_BaseVehicle> VehicleClass;
    
public:
    void SetDisplayName(FString Name);
    void SetVehicleClass(TSubclassOf<AST_BaseVehicle> NewVehicleClass);
    
protected:
    virtual void NativeConstruct() override;
    
private:
    UFUNCTION()
    void OnVehicleButtonClicked();
};

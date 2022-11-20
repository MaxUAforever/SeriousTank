#pragma once

#include "GameFramework/ST_BaseHUD.h"
#include "ST_MainMenuHUD.generated.h"

class UUserWidget;

UCLASS()
class SERIOUSTANK_API AST_MainMenuHUD : public AST_BaseHUD
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> GarageWidgetClass;
    
private:
    UUserWidget* GarageWidget;
    
public:
    void SwitchToGarageWidget();
    virtual void SwitchFromSettingsWidget() override;

protected:
    virtual void BeginPlay() override;
};

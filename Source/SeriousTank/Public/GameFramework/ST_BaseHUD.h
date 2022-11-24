#pragma once

#include "GameFramework/HUD.h"
#include "ST_BaseHUD.generated.h"

class UUserWidget;

UCLASS()
class SERIOUSTANK_API AST_BaseHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> GameUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> SettingsWidgetClass;

protected:
	UUserWidget* GameUIWidget;
	UUserWidget* SettingsWidget;

private:
    UUserWidget* CurrentWidget;

public:
    void SwitchToMainWidget();
	void SwitchToSettingsWidget();
    
    virtual void SwitchFromSettingsWidget() {};
    
protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
    
	UUserWidget* CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass);
    void SwitchToWidget(UUserWidget* NeededWidget);
};

#pragma once

#include "GameFramework/ST_BaseHUD.h"
#include "ST_GameplayHUD.generated.h"

UCLASS()
class SERIOUSTANK_API AST_GameplayHUD : public AST_BaseHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> FinalScoreWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	UUserWidget* FinalScoreWidget;

	UPROPERTY()
	UUserWidget* PauseWidget;

public:
    void SwitchToPauseWidget();
    void SwitchToFinalScoreWidget();
    
    virtual void SwitchFromSettingsWidget() override;
    
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnGameTimeEnded();

	UFUNCTION()
	void OnGameIsPaused(bool IsPaused);
};

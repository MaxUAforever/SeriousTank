#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_MainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UScrollBox;
class USoundCue;
class UST_GameplayLevelCardWidget;
struct FGameplayLevelInfo;

UCLASS()
class SERIOUSTANK_API UST_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseGameplayLevelButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitGameButton;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UButton* GarageButton;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* ChooseLevelButtonCaption;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UScrollBox* LevelsScrollBox;
    
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* BackgroundMusic;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UST_GameplayLevelCardWidget> LevelCardWidgetClass;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartGameButtonClicked();

	UFUNCTION()
	void OnChooseLevelButtonClicked();

	UFUNCTION()
	void OnLevelIsChoosen(int32 LevelIndex);

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

    UFUNCTION()
    void OnGarageButtonClicked();
    
	void AddLevelCardWidget(const FGameplayLevelInfo& LevelInfo, int32 LevelIndex);
	void SetCurrentLevelCaption(int32 LevelIndex);
};

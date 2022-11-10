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
	UTextBlock* ChooseLevelButtonCaption;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* LevelsScrollBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UST_GameplayLevelCardWidget> LevelCardWidgetClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitGameButton;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* BackgroundMusic;

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
	void OnExitButtonClicked();

	void AddLevelCardWidget(const FGameplayLevelInfo& LevelInfo, int32 LevelIndex);
	void SetCurrentLevelCaption(int32 LevelIndex);
};

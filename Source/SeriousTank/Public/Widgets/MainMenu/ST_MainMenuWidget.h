#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_MainMenuWidget.generated.h"

class UButton;
class UTextBlock;
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

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartGameButtonClicked();
};

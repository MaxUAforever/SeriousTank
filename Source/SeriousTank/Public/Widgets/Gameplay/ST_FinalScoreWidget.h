#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_FinalScoreWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

UCLASS()
class SERIOUSTANK_API UST_FinalScoreWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ScoreValueBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimeValueBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TargetsValueBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayAgainButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MenuButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* ScoreVerticalBox;

public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnPlayAgainButtonClicked();

	UFUNCTION()
	void OnMenuButtonClicked();

	UFUNCTION()
	void OnGameIsOver();
};

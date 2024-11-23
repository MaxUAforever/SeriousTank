#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_GameplayLevelCardWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class SERIOUSTANK_API UST_GameplayLevelCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChooseLevelButtonClicked, int32)
	FOnChooseLevelButtonClicked OnLevelButtonClicked;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseLevelButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LevelNameBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LevelImage;

	UPROPERTY(EditAnywhere)
	int32 LevelIndex;

public:
	virtual void NativeConstruct() override;

	void SetLevelName(FText LevelName);
	void SetLevelIndex(int32 NewLevelIndex);

private:
	UFUNCTION()
	void OnChooseLevelButtonClicked();
};

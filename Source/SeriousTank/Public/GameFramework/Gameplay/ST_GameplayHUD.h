#pragma once

#include "GameFramework/ST_BaseHUD.h"
#include "ST_GameplayHUD.generated.h"

UCLASS()
class SERIOUSTANK_API AST_GameplayHUD : public AST_BaseHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> FinalScoreWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	UUserWidget* FinalScoreWidget;

	UPROPERTY()
	UUserWidget* PauseWidget;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnGameTimeEnded();

	UFUNCTION()
	void OnGameIsPaused(bool IsPaused);
};

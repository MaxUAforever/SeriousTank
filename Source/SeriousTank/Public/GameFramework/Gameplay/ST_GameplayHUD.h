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

	UPROPERTY()
	UUserWidget* FinalScoreWidget;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnGameTimeEnded();
};

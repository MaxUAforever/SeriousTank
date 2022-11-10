#pragma once

#include "GameFramework/HUD.h"
#include "ST_BaseHUD.generated.h"

class UUserWidget;

UCLASS()
class SERIOUSTANK_API AST_BaseHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> GameUIClass;

	UPROPERTY()
	UUserWidget* GameUIWidget;

protected:
	virtual void BeginPlay() override;

	UUserWidget* CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass);
};

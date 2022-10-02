#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

class UTextBlock;

UCLASS()
class SERIOUSTANK_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MainWeaponReloadingBlock;
};

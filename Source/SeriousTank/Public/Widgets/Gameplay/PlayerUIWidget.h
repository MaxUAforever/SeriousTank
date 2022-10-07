#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

class UST_WeaponReloadingWidget;

UCLASS()
class SERIOUSTANK_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UST_WeaponReloadingWidget* FirstWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UST_WeaponReloadingWidget* SecondWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UST_WeaponReloadingWidget* ThirdWeaponReloadingWidget;
};

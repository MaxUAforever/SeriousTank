#pragma once

#include "Components/Button.h"
#include "ST_MainButton.generated.h"


UCLASS()
class SERIOUSTANK_API UST_MainButton : public UButton
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FButtonStyle SelectedButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FButtonStyle NotSelectedButtonStyle;

public:
	virtual void OnWidgetRebuilt() override;

private:
	UFUNCTION()
	void OnNavigationKeyPressed();

public:
	UFUNCTION(BlueprintCallable)
	void UpdateState(bool bIsMouseEvent);
};

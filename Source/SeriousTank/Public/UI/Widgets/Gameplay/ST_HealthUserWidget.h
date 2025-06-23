// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_HealthUserWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SERIOUSTANK_API UST_HealthUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetMaxHealthValue(float InMaxHealth);
	void SetHealthValue(float InHealth);

	void SetProgressBarColor(const FLinearColor& InColor);

private:
	void UpdateView();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HealthValueBlock;

private:
	float MaxHealthValue = 100.f;
	float CurrentHealthValue = 100.f;
};

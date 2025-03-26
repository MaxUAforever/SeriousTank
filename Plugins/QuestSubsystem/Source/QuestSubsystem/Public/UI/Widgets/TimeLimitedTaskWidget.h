// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TimeLimitedTaskWidget.generated.h"

class UTextBlock;

UCLASS()
class QUESTSUBSYSTEM_API UTimeLimitedTaskWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetRemainingTime(float InRemainingTime);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RemainingTimeBlock;
};

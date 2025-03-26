// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_PreStartCountdownWidget.generated.h"

class UAudioComponent;
class USoundCue;
class UTextBlock;

UCLASS()
class SERIOUSTANK_API UST_PreStartCountdownWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void UpdatePreStartTime(int32 NewTime);

	UFUNCTION()
	void HidePreStartTimeBlock();

protected:
	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* PreStartTimeBlock;

	UPROPERTY(EditAnyWhere, Category = "Audio")
	USoundCue* CountdownSound;

	UPROPERTY(EditAnyWhere, Category = "Audio")
	USoundCue* StartGameSound;

	UPROPERTY(VisibleAnyWhere, Category = "Audio")
	UAudioComponent* UISoundsComponent;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QuestSubsystem/Public/UI/Widgets/TimeLimitedTaskWidget.h"
#include "ST_TargetShootingWidget.generated.h"

class UTextBlock;

UCLASS()
class SERIOUSTANK_API UST_TargetShootingWidget : public UTimeLimitedTaskWidget
{
	GENERATED_BODY()
	
public:
	void SetScore(int32 InScore);

protected:
	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* ScoreBlock;
};

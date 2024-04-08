// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "SoldierInputsDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(Blueprintable)
class SERIOUSTANK_API USoldierInputsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputMappingContext* SoldierGameplayInputContext;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SprintInputAction;
};

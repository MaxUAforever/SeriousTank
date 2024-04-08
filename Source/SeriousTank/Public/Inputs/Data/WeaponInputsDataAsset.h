// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "WeaponInputsDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(Blueprintable)
class SERIOUSTANK_API UWeaponInputsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputMappingContext* WeaponsInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* FireInputAction;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* ReloadInputAction;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SwitchToFirstWeaponInputAction;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SwitchToSecondWeaponInputAction;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SwitchToThirdWeaponInputAction;

};

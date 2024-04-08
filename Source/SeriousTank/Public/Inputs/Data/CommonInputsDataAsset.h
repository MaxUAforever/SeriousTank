#pragma once

#include "Engine/DataAsset.h"
#include "CommonInputsDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(Blueprintable)
class SERIOUSTANK_API UCommonInputsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputMappingContext* CommonGameplayInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* MoveForwardInputAction;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* MoveRightInputAction;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* RotateCameraInputAction;
};

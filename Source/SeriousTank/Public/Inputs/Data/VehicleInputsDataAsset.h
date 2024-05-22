#pragma once

#include "Engine/DataAsset.h"
#include "VehicleInputsDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SERIOUSTANK_API UVehicleInputsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputMappingContext* VehicleInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* ExitVehicleInputAction;

};

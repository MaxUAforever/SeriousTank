#pragma once

#include "UObject/Object.h"
#include "ST_VehicleSpawnManager.generated.h"

struct FVehicleInfo;

UCLASS(BlueprintType)
class SERIOUSTANK_API UST_VehicleSpawnManager : public UObject
{
	GENERATED_BODY()

private:
	AST_BaseVehicle* Vehicle;

public:
	void SpawnVehicle(FVehicleInfo VehicleInfo);

private:
	void DisableSoundsOnVehicle();
};

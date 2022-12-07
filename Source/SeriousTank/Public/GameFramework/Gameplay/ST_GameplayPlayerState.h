#pragma once

#include "GameFramework/PlayerState.h"
#include "ST_GameplayPlayerState.generated.h"

struct FVehicleInfo;

UCLASS()
class SERIOUSTANK_API AST_GameplayPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
    FVehicleInfo VehicleInfo;
	
public:
    FVehicleInfo GetVehicleInfo() const { return VehicleInfo; };
    void SetVehicleInfo(FVehicleInfo NewVehicleInfo) { VehicleInfo = NewVehicleInfo; };

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
};

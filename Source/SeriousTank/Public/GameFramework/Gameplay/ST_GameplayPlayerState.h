#pragma once

#include "GameFramework/PlayerState.h"
#include "ST_GameplayPlayerState.generated.h"

struct FVehicleInfo;
struct FSoldierInfo;

UCLASS()
class SERIOUSTANK_API AST_GameplayPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
    FVehicleInfo VehicleInfo;
	
	// TODO: make private and remove uproperty after testing
	UPROPERTY(EditDefaultsOnly)
	FSoldierInfo SoldierInfo;

public:
    FVehicleInfo GetVehicleInfo() const { return VehicleInfo; };
    void SetVehicleInfo(FVehicleInfo NewVehicleInfo) { VehicleInfo = NewVehicleInfo; };

	FSoldierInfo GetSoldierInfo() const { return SoldierInfo; };
	void SetSoldierInfo(FSoldierInfo NewSoldierInfo) { SoldierInfo = NewSoldierInfo; };

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
};

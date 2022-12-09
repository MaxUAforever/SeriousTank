#pragma once

#include "GameFramework/MainMenu/Utils/ST_PlayerStateSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "ST_MainMenuPlayerState.generated.h"

class AST_BaseVehicle;
struct FVehicleInfo;

UCLASS()
class SERIOUSTANK_API AST_MainMenuPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    DECLARE_DELEGATE_OneParam(FOnCurrentVehicleChanged, FVehicleInfo)
    FOnCurrentVehicleChanged OnCurrentVehicleChanged;
    
protected:
    // TODO: Move to private when adding of vehicles is implemented.
    UPROPERTY(EditDefaultsOnly)
    TArray<FVehicleInfo> AvailableVehicles;
	
private:
    int32 CurrentVehicleIndex = 0;
    
public:
    virtual void BeginPlay() override;
    
    TArray<FVehicleInfo> GetVehicles() const { return AvailableVehicles; };
    FVehicleInfo GetVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass) const;
    FVehicleInfo GetVehicle(int32 VehicleIndex) const;
    
    int32 GetCurrentVehicleIndex() const { return CurrentVehicleIndex; };
    FVehicleInfo GetCurrentVehicle() const;
    void SetCurrentVehicle(int32 NewVehicleIndex);
    
    void SetVehicles(TArray<FVehicleInfo> Vehicles);
    void AddVehicle(FVehicleInfo VehicleInfo);
    void RemoveVehicle(FVehicleInfo VehicleInfo);
    
protected:
    virtual void CopyProperties(APlayerState* PlayerState) override;
};

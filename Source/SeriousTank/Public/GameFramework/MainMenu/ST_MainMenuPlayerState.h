#pragma once

#include "GameFramework/PlayerState.h"

#include "Templates/SubclassOf.h"

#include "ST_MainMenuPlayerState.generated.h"

class AST_BaseVehicle;

UCLASS()
class SERIOUSTANK_API AST_MainMenuPlayerState : public APlayerState
{
	GENERATED_BODY()
    
protected:
    // TODO: Move to private when adding of vehicles is implemented.
    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<AST_BaseVehicle>> AvailableVehicleClasses;
	
public:
    TArray<TSubclassOf<AST_BaseVehicle>> GetVehicles() const { return AvailableVehicleClasses; };
    
    void AddVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass);
    void RemoveVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass);
};

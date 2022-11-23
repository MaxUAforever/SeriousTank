#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"

#include "Actors/Pawns/ST_BaseVehicle.h"

void AST_MainMenuPlayerState::AddVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass)
{
    AvailableVehicleClasses.Add(VehicleClass);
}

void AST_MainMenuPlayerState::RemoveVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass)
{
    AvailableVehicleClasses.Remove(VehicleClass);
}

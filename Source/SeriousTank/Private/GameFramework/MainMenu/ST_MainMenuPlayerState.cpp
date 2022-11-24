#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"

FVehicleInfo AST_MainMenuPlayerState::GetVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass) const
{
    for (const FVehicleInfo& VehicleInfo : AvailableVehicles)
    {
        if (VehicleInfo.VehicleClass == VehicleClass)
        {
            return VehicleInfo;
        }
    }
    
    return FVehicleInfo{};
}

FVehicleInfo AST_MainMenuPlayerState::GetVehicle(int32 VehicleIndex) const
{
    return VehicleIndex >= 0 && VehicleIndex < AvailableVehicles.Num() ? AvailableVehicles[VehicleIndex] : FVehicleInfo{};
}

FVehicleInfo AST_MainMenuPlayerState::GetCurrentVehicle() const
{
    return GetVehicle(CurrentVehicleIndex);
}

void AST_MainMenuPlayerState::SetCurrentVehicle(int32 NewVehicleIndex)
{
    CurrentVehicleIndex = NewVehicleIndex;
    
    OnCurrentVehicleChanged.ExecuteIfBound(GetCurrentVehicle());
}

void AST_MainMenuPlayerState::AddVehicle(FVehicleInfo VehicleInfo)
{
    AvailableVehicles.Add(VehicleInfo);
}

void AST_MainMenuPlayerState::RemoveVehicle(FVehicleInfo VehicleInfo)
{
    AvailableVehicles.Remove(VehicleInfo);
}

void AST_MainMenuPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);
    
    if (AST_GameplayPlayerState* GameplayPlayerState = Cast<AST_GameplayPlayerState>(PlayerState))
    {
        GameplayPlayerState->SetVehicleInfo(GetCurrentVehicle());
    }
}

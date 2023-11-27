#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"

void AST_GameplayPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AST_GameplayPlayerState* GameplayPlayerState = Cast<AST_GameplayPlayerState>(PlayerState))
	{
		GameplayPlayerState->SetVehicleInfo(VehicleInfo);
		GameplayPlayerState->SetSoldierInfo(SoldierInfo);
	}
}
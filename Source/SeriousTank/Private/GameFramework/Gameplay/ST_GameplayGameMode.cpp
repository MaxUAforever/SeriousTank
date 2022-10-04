#include "GameFramework/Gameplay/ST_GameplayGameMode.h"

#include "Actors/Pawns/ST_BaseTrackedVehicle.h"

void AST_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();

	DefaultPawnClass = AST_BaseTrackedVehicle::StaticClass();
}
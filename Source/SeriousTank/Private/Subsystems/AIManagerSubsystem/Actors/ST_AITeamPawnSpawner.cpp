#include "Subsystems/AIManagerSubsystem/Actors/ST_AITeamPawnSpawner.h"

#include "Components/Teams/ST_TeamOwnershipComponent.h"
#include "Engine/World.h"
#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"

AST_AITeamPawnSpawner::AST_AITeamPawnSpawner()
{
	TeamOwnershipComponent = CreateDefaultSubobject<UST_TeamOwnershipComponent>("TeamOwnershipComponent");
}

void AST_AITeamPawnSpawner::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	UST_AITeamsManagerSubsystem* AITeamsManagerSubsystem = World->GetSubsystem<UST_AITeamsManagerSubsystem>();
	if (IsValid(AITeamsManagerSubsystem))
	{
		return;
	}

	AITeamsManagerSubsystem->RegisterTeamSpawner(this);
}
#include "Subsystems/AIManagerSubsystem/Actors/ST_AITeamPawnSpawner.h"

#include "Components/Teams/ST_TeamOwnershipComponent.h"
#include "Engine/World.h"
#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"

AST_AITeamPawnSpawner::AST_AITeamPawnSpawner()
{
	TeamOwnershipComponent = CreateDefaultSubobject<UST_TeamOwnershipComponent>("TeamOwnershipComponent");
}

uint8 AST_AITeamPawnSpawner::GetTeamId() const
{
	return IsValid(TeamOwnershipComponent) ? TeamOwnershipComponent->GetGenericTeamId().GetId() : 0;
}

void AST_AITeamPawnSpawner::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid World context"), *GetClass()->GetName(), TEXT("BeginPlay"));
		return;
	}

	UST_AITeamsManagerSubsystem* AITeamsManagerSubsystem = World->GetSubsystem<UST_AITeamsManagerSubsystem>();
	if (!IsValid(AITeamsManagerSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid AITeamsManagerSubsystem context"), *GetClass()->GetName(), TEXT("BeginPlay"));
		return;
	}
	
	if (GetSpawnOwner() != AITeamsManagerSubsystem)
	{
		AITeamsManagerSubsystem->RegisterTeamSpawner(this);
	}
}
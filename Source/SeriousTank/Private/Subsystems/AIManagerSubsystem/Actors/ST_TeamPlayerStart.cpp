#include "Subsystems/AIManagerSubsystem/Actors/ST_TeamPlayerStart.h"

#include "Components/Teams/ST_TeamOwnershipComponent.h"
#include "Engine/World.h"
#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"

AST_TeamPlayerStart::AST_TeamPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamOwnershipComponent = CreateDefaultSubobject<UST_TeamOwnershipComponent>("TeamOwnershipComponent");
}

uint8 AST_TeamPlayerStart::GetTeamId() const
{
	return IsValid(TeamOwnershipComponent) ? TeamOwnershipComponent->GetGenericTeamId().GetId() : 0;
}
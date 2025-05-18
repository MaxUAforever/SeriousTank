#include "Components/Teams/ST_TeamOwnershipComponent.h"

UST_TeamOwnershipComponent::UST_TeamOwnershipComponent()
	: TeamID(FGenericTeamId::NoTeam)
{
}

void UST_TeamOwnershipComponent::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID == NewTeamID)
	{
		return;
	}
		
	TeamID = NewTeamID;
	OnTeamOwnershipChangedDelegate.Broadcast(this, TeamID);
}

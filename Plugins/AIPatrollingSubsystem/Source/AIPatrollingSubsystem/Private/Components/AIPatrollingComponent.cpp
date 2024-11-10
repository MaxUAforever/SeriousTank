#include "Components/AIPatrollingComponent.h"

#include "AIPatrollingSubsystem.h"

void UAIPatrollingComponent::BeginPlay()
{
	Super::BeginPlay();

	UAIPatrollingSubsystem* PatrolingSubsystem = GetWorld()->GetSubsystem<UAIPatrollingSubsystem>();
	if (PatrolingSubsystem)
	{
		PatrolingSubsystem->RegisterAgent(this);
		if (bStartWithPatrollingActive)
		{
			PatrolingSubsystem->StartPatrolling(this);
		}
	}
}

void UAIPatrollingComponent::SetIsPatrollingActive(bool bInIsActive)
{
	if (bIsPatrollingActive == bInIsActive)
	{
		return;
	}

	bIsPatrollingActive = bInIsActive;
	OnIsActiveChanged.Broadcast(bIsPatrollingActive);
}

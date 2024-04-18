#include "Subsystems/HealthSubsystem/ST_DamageDealingComponent.h"

#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

void UST_DamageDealingComponent::StartDealingDamage(AActor* ContactActor, const FVector& DealingLocation)
{
	if (UST_HealthSubsystem* HealthSubsystem = GetWorld()->GetSubsystem<UST_HealthSubsystem>())
	{
		HealthSubsystem->DealDamage(this, ContactActor, DealingLocation);
	}
}

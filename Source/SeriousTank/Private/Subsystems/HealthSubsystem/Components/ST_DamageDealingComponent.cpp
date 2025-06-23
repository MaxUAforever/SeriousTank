#include "Subsystems/HealthSubsystem/Components/ST_DamageDealingComponent.h"

#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

void UST_DamageDealingComponent::StartDealingDamage(AController* DamageDealer, AActor* DamageReciever, const FVector& DealingLocation)
{
	if (UST_HealthSubsystem* HealthSubsystem = GetWorld()->GetSubsystem<UST_HealthSubsystem>())
	{
		HealthSubsystem->DealDamage(this, DamageDealer, DamageReciever, DealingLocation);
	}
}

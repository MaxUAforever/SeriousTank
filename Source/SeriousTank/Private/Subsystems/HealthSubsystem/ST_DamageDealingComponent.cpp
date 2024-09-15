#include "Subsystems/HealthSubsystem/ST_DamageDealingComponent.h"

#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"

void UST_DamageDealingComponent::StartDealingDamage(AActor* ContactActor, const FVector& DealingLocation)
{
	if (UST_HealthComponent* HealthComponent = ContactActor->GetComponentByClass<UST_HealthComponent>())
	{
		HealthComponent->AddHealthValue(DamageValue * -1.f);
	}
}

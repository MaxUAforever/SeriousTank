#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_DamageDealingComponent.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"

bool UST_HealthSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return Super::ShouldCreateSubsystem(Outer) && World && !World->IsNetMode(NM_Client);
}

void UST_HealthSubsystem::RegisterHealthActor(AActor* Actor)
{
	HealthActors.Add(Actor);
}

void UST_HealthSubsystem::DealDamage(UST_DamageDealingComponent* DamageDealingComponent, AActor* ContactActor, const FVector& DealingLocation)
{
	if (!DamageDealingComponent)
	{
		return;
	}

	if (HealthActors.Contains(ContactActor))
	{
		if (UST_HealthComponent* HealthComponent = ContactActor->GetComponentByClass<UST_HealthComponent>())
		{
			HealthComponent->AddHealthValue(DamageDealingComponent->GetDamageValue() * -1.f);
		}
	}
}
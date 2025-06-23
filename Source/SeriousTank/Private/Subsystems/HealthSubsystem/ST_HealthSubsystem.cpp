#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Subsystems/HealthSubsystem/Components/ST_DamageDealingComponent.h"
#include "Subsystems/HealthSubsystem/Components/ST_HealthComponent.h"

bool UST_HealthSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return Super::ShouldCreateSubsystem(Outer) && World && !World->IsNetMode(NM_Client);
}

void UST_HealthSubsystem::RegisterHealthActor(AActor* Actor)
{
	RegisteredHealthActors.Add(Actor);
}

void UST_HealthSubsystem::DealDamage(UST_DamageDealingComponent* DamageDealingComponent, AController* DamageDealer, AActor* DamageReciever, const FVector& DealingLocation)
{
	if (!DamageDealingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_HealthSubsystem::DealDamage: DamageDealingComponent is null"));
		return;
	}

	if (!IsValid(DamageReciever) || !IsValid(DamageDealer))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_HealthSubsystem::DealDamage: DamageReciever or DamageDealer is invalid"));
		return;
	}

	if (!RegisteredHealthActors.Contains(DamageReciever))
	{
		UE_LOG(LogTemp, Display, TEXT("UST_HealthSubsystem::DealDamage: DamageReciever is not registered in the health subsystem"));
		return;
	}

	if (UST_HealthComponent* HealthComponent = DamageReciever->GetComponentByClass<UST_HealthComponent>())
	{
		HealthComponent->AddHealthValue(DamageDealingComponent->GetDamageValue() * -1.f);

		if (FMath::IsNearlyZero(HealthComponent->GetCurrentHealth()))
		{
			OnActorEliminatedDelegate.Broadcast(DamageDealer, DamageReciever);
		}
	}
}
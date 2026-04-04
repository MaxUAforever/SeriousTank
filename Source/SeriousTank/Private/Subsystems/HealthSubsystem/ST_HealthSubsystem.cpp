#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Subsystems/HealthSubsystem/Components/ST_DamageDealingComponent.h"
#include "Subsystems/HealthSubsystem/Components/ST_HealthComponent.h"

void UST_HealthSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	FST_DamageDealingEventHandler::BusConnect(this);
}

void UST_HealthSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	Super::OnWorldEndPlay(InWorld);

	FST_DamageDealingEventHandler::BusDisconnect();
}

bool UST_HealthSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return Super::ShouldCreateSubsystem(Outer) && World && !World->IsNetMode(NM_Client);
}

void UST_HealthSubsystem::RegisterHealthActor(AActor* Actor)
{
	RegisteredHealthActors.Add(Actor);
}

bool UST_HealthSubsystem::DealDamage(AController* DamageDealer, AActor* DamageReciever, float DamageValue)
{
	if (!IsValid(DamageReciever) || !IsValid(DamageDealer))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: DamageReciever or DamageDealer is invalid"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	if (!RegisteredHealthActors.Contains(DamageReciever))
	{
		UE_LOG(LogTemp, Display, TEXT("%s: DamageReciever is not registered in the health subsystem"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	UST_HealthComponent* HealthComponent = DamageReciever->GetComponentByClass<UST_HealthComponent>();
	if (!IsValid(HealthComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: HealthComponent is not valid on the DamageReciever"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}
	
	HealthComponent->AddHealthValue(DamageValue * -1.f);
	if (FMath::IsNearlyZero(HealthComponent->GetCurrentHealth()))
	{
		OnActorEliminatedDelegate.Broadcast(DamageDealer, DamageReciever);
	}
	
	return true;
}

void UST_HealthSubsystem::OnDamageDealingAttempt(AController* DamageDealer, AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo, float DemageDealt)
{
	UE_LOG(LogTemp, Display, TEXT("%s: DamageDealer: %s, DamageReciever: %s, DamageDealingLocation: %s, DamageDealingDirection: %s, ImpactNormal: %s, DamageDealt: %f"), ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(DamageDealer), *GetNameSafe(DamageReciever), *DamageDealingInfo.DealingLocation.ToString(), *DamageDealingInfo.DamageDealerDirection.ToString(), *DamageDealingInfo.ImpactNormal.ToString(), DemageDealt);
}

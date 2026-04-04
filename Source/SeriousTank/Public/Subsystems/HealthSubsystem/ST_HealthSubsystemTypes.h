#pragma once

#include "CoreMinimal.h"
#include "Core/Bus/ST_Bus.h"

class UPrimitiveComponent;
class AController;
class AActor;

// Struct containing necessary information for calculating and applying damage to a damage receiver.
struct FST_DamageDealingParameters
{
	const UPrimitiveComponent* ImpactedComponent;
	FVector DealingLocation;
	FVector DamageDealerDirection;
	FVector ImpactNormal;
};

struct FST_HealthSubsystemEvents
{
	virtual ~FST_HealthSubsystemEvents() = default;

	virtual void OnDamageDealingAttempt(AController* DamageDealer, AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo, float DemageDealt) {};
};

using FST_DamageDealingEventBus = ST_Bus<FST_HealthSubsystemEvents>;
using FST_DamageDealingEventHandler = ST_Bus<FST_HealthSubsystemEvents>::Handler;
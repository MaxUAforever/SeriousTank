#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "ST_HealthSubsystem.generated.h"

class AController;
class UST_DamageDealingComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActorEliminatedDelegate, const AController* /*KillerPlayer*/, const AActor* /*EliminatedActor*/);

UCLASS()
class SERIOUSTANK_API UST_HealthSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FOnActorEliminatedDelegate OnActorEliminatedDelegate;

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void RegisterHealthActor(AActor* Actor);

	void DealDamage(UST_DamageDealingComponent* DamageDealingComponent, AController* DamageDealer, AActor* DamageReciever, const FVector& DealingLocation);

private:
	TSet<TWeakObjectPtr<AActor>> RegisteredHealthActors;
};

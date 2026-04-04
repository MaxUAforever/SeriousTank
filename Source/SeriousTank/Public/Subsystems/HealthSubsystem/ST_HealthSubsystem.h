#pragma once

#include "Subsystems/HealthSubsystem/ST_HealthSubsystemTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "ST_HealthSubsystem.generated.h"

class AController;
class UST_DamageDealingComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActorEliminatedDelegate, const AController* /*KillerPlayer*/, const AActor* /*EliminatedActor*/);

UCLASS()
class SERIOUSTANK_API UST_HealthSubsystem : public UWorldSubsystem, public FST_DamageDealingEventHandler
{
	GENERATED_BODY()

public:
	FOnActorEliminatedDelegate OnActorEliminatedDelegate;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void RegisterHealthActor(AActor* Actor);

	bool DealDamage(AController* DamageDealer, AActor* DamageReciever, float DamageValue);

	virtual void OnDamageDealingAttempt(AController* DamageDealer, AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo, float DemageDealt) override;

private:
	TSet<TWeakObjectPtr<AActor>> RegisteredHealthActors;
};

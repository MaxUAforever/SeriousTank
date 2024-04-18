#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "ST_HealthSubsystem.generated.h"

class UST_DamageDealingComponent;

UCLASS()
class SERIOUSTANK_API UST_HealthSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TSet<AActor*> HealthActors;

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void RegisterHealthActor(AActor* Actor);

	void DealDamage(UST_DamageDealingComponent* DamageDealingComponent, AActor* ContactActor, const FVector& DealingLocation);
};

#pragma once

#include "Components/ActorComponent.h"
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactTypes.h"
#include "ST_PhysicalImpulseComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = Physics)
class SERIOUSTANK_API UST_PhysicalImpulseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ApplyPhysicalImpulse(AActor* ImpactedActor, const FST_PhysicalImpactParameters& ImpactParameters);

	float GetImpulseStrength() const { return ImpulseStrength; }

protected:
	UPROPERTY(EditDefaultsOnly)
	float ImpulseStrength = 1000.f;
};

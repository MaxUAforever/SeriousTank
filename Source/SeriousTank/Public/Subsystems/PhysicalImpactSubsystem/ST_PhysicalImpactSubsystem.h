#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactTypes.h"
#include "ST_PhysicalImpactSubsystem.generated.h"

class UST_PhysicalImpulseComponent;

UCLASS()
class SERIOUSTANK_API UST_PhysicalImpactSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void ApplyPhysicalImpact(UST_PhysicalImpulseComponent* PhysicalImpulseComponent, AActor* ImpactedActor, const FST_PhysicalImpactParameters& ImpactParameters);

	bool IsPhysicalImpactEnabled() const { return bIsPhysicalImpactEnabled; }
	void SetPhysicalImpactEnabled(bool bEnabled) { bIsPhysicalImpactEnabled = bEnabled; }

private:
	bool bIsPhysicalImpactEnabled = true;
};

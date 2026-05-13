#pragma once

// Engine includes
#include "Logging/LogMacros.h"
#include "Math/Vector.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPhysicalImpact, Log, All);

struct FST_PhysicalImpactParameters
{
	FVector ImpactLocation;
	FVector ImpactForceDirection;
	float ImpactForceStrength;
};

struct FST_PhysicalAnimationProperties
{
	FName ProfileName;
	FName BoneName;
	bool bIncludeBone;
};

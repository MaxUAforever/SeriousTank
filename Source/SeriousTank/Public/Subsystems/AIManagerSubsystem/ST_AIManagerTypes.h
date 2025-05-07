#pragma once

#include "Types/SlateEnums.h"
#include "ST_AIManagerTypes.generated.h"

UENUM(BlueprintType)
enum class EAIInteractionType : uint8
{
	RefillingAmmo,
	PickingUpWeapon,
	TakingVehicle,
	Count UMETA(Hidden) // Used only for iteration
};

ENUM_RANGE_BY_COUNT(EAIInteractionType, EAIInteractionType::Count);

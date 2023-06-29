#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "ST_AbilitySet.generated.h"

class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EST_TargetingType : uint8
{
	/* If ability should be activated immediately without targeting. */
	None,

	/* If ability should be confirmed without targeting. */
	Auto,

	/* If ability should be targeted to any actor. */
	Actor,

	/* If ability should be targeted to area. */
	Area
};

USTRUCT(BlueprintType)
struct FST_GameplayAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	EST_TargetingType TargetingType;
};


USTRUCT(BlueprintType)
struct FST_AbilitySet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FST_GameplayAbilityInfo GameplayAbilityInfo;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClasses;

	// TODO: Add AttributeSet for equipment properties
};

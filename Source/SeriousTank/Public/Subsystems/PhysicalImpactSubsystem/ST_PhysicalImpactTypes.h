#pragma once

// Engine includes
#include "Logging/LogMacros.h"
#include "Math/Vector.h"

#include "ST_PhysicalImpactTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPhysicalImpact, Log, All);

class UCurveFloat;

UENUM(BlueprintType)
enum class EST_PhysicalReactionType : uint8
{
	None,
	Hit,
	Ragdoll
};

struct FST_PhysicalImpactParameters
{
	FVector ImpactLocation;
	FVector ImpactForceDirection;
	float ImpactForceStrength;
	EST_PhysicalReactionType ExpectedReactionType;
};

USTRUCT(BlueprintType)
struct FST_PhysicalAnimationProperties
{
	GENERATED_BODY()

	// The name of the physical animation profile to apply to the impacted bone and its children. 
	// This profile should be defined in the Physical Animation section of the impacted character's skeletal mesh component.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Animation")
	FName ProfileName = NAME_None;

	// The name of the bone to apply the physical animation to.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Animation")
	FName BoneName = NAME_None;

	// If true, the bone itself will be included in the physical animation, otherwise only its children will be affected.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Animation")
	bool bIncludeBone = false;

	// If true, the physical reaction will be applied over time using the provided curve. 
	// If false, the physical reaction will be applied instantly and remain until reset.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Reaction")
	bool bIsTimedReaction = false;

	// Curve that defines the strength of the physical reaction over time. 
	// The curve's time range should start at 0 and end at the desired duration of the reaction. 
	// The curve's value range should be from 0 to 1, where 0 means no reaction and 1 means full reaction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Reaction", meta = (EditCondition = "bIsTimedReaction"))
	TObjectPtr<UCurveFloat> ReactionForceCurve = nullptr;
};

USTRUCT(BlueprintType)
struct FST_PhysicalReactionSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Reaction")
	EST_PhysicalReactionType ReactionType = EST_PhysicalReactionType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physical Reaction")
	FST_PhysicalAnimationProperties AnimationProperties;
};
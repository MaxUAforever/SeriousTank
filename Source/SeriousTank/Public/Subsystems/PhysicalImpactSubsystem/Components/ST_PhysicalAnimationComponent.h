#pragma once

#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactTypes.h"
#include "ST_PhysicalAnimationComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = Physics)
class SERIOUSTANK_API UST_PhysicalAnimationComponent : public UPhysicalAnimationComponent
{
	GENERATED_BODY()

public:
	UST_PhysicalAnimationComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetReactionType(EST_PhysicalReactionType ReactionType);
	void ApplyPhysicalImpact(const FST_PhysicalImpactParameters& ImpactParameters);

	EST_PhysicalReactionType GetCurrentReactionType() const { return CurrentReactionType; }

private:
	void InitializePhysicalReactionPropertiesMap();
	void InitializeCurrentImpactProperties();

	void StopCurrentPhysicalReaction();
	bool HasActivePhysicalReaction() const;
	
protected:
	/** 
	 * Array of physical reaction settings that define how the component should respond to different types of physical impacts.
	 * Should be populated in the editor, so it can be used to initialize the PhysicalReactionPropertiesMap for quick access during gameplay.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FST_PhysicalReactionSettings> PhysicalReactionSettingsArray;

private:
	TMap<EST_PhysicalReactionType, FST_PhysicalAnimationProperties> PhysicalReactionPropertiesMap;

	EST_PhysicalReactionType CurrentReactionType;

	/** Current force curve parameters, that are initialized when a timed physical impact is applied. */
	float ImpactCurveMinTime;
	float ImpactCurveMaxTime;
	float ImpactCurveDuration;

	float CurrentImpactCurveTime;

	bool bIsInitialized = false;
};

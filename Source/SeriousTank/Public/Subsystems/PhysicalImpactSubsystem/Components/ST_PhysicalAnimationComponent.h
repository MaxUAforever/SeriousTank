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

	bool IsEnabled() const { return bIsEnabled; }
	void SetEnabled(bool bInIsEnabled);

public:
	void UpdateAnimationProperties(const FST_PhysicalAnimationProperties& AnimationProperties);
	void ApplyPhysicalImpact(const FST_PhysicalImpactParameters& ImpactParameters, float ImpactForce);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DefaultProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DefaultImpactBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDefaultIncludeBone = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultImpactReactionTime = 2.f;

private:
	FST_PhysicalAnimationProperties CurrentAnimationProperties;

	float RemainingImpactReactionTime;

	bool bIsEnabled = true;
};

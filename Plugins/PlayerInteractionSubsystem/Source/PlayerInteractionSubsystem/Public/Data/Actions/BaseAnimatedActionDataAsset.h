#pragma once

#include "Data/Actions/BaseInteractionActionDataAsset.h"
#include "BaseAnimatedActionDataAsset.generated.h"

class UAnimMontage;

/**
 *
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UBaseAnimatedActionDataAsset : public UBaseInteractionActionDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Animated Action Settings")
	float ActivationMontageCompleteDelay = 0.f;

	UPROPERTY(EditAnywhere, Category = "Animated Action Settings")
	float DeactivationMontageCompleteDelay = 0.f;
};

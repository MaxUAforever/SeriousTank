#pragma once

#include "Engine/DataAsset.h"
#include "BaseInteractionActionDataAsset.generated.h"

class UAnimMontage;

/**
 *
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UBaseInteractionActionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Base Action Settings")
	FText ActionDescription;

	UPROPERTY(EditAnywhere, Category = "Base Action Settings")
	TObjectPtr<UAnimMontage> ActivationMontage;

	UPROPERTY(EditAnywhere, Category = "Base Action Settings")
	TObjectPtr<UAnimMontage> DeactivationMontage;

	UPROPERTY(EditAnywhere, Category = "Base Action Settings")
	bool bShouldBeDeactivated = true;
};

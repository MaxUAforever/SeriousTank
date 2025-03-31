#pragma once

#include "Data/Actions/BaseInteractionActionDataAsset.h"
#include "BaseInterruptibleActionDataAsset.generated.h"

class UAnimMontage;

UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UBaseInterruptibleActionDataAsset : public UBaseInteractionActionDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Interruptible Action Settings")
	float ActivationTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Interruptible Action Settings")
	float DeactivationTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Interruptible Action Settings")
	FText InterruptionCaption = FText::FromString("Stop action");
};

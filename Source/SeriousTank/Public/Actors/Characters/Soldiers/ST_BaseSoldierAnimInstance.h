#pragma once

#include "Animation/AnimInstance.h"
#include "ST_BaseSoldierAnimInstance.generated.h"

enum class ECharacterMovingType : uint8;

UCLASS()
class SERIOUSTANK_API UST_BaseSoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	ECharacterMovingType MovingType;

public:
	virtual void NativeInitializeAnimation() override;

private:
	FORCEINLINE void OnMovementTypeChanged(ECharacterMovingType InMovingType) { MovingType = InMovingType; };
};

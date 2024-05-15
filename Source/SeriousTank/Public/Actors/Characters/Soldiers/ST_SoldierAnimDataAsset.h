#pragma once

#include "Core/Animation/ST_AnimMontageDataAsset.h"
#include "ST_SoldierAnimDataAsset.generated.h"

class UAnimMontage;

UCLASS()
class SERIOUSTANK_API UST_SoldierAnimDataAsset : public UST_AnimMontageDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TurnLeftMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TurnRightMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TwoHandsWeaponFireMontage;

    UPROPERTY(EditDefaultsOnly)
    UAnimMontage* TwoHandsWeaponReloadingMontage;

	UPROPERTY(EditDefaultsOnly)
    UAnimMontage* SwitchWeaponMontage;

	UPROPERTY(EditDefaultsOnly)
    UAnimMontage* HitReactionMontage;
};

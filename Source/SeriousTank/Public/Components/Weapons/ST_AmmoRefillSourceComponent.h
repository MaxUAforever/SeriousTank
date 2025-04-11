#pragma once

#include "Components/ActorComponent.h"
#include "Actors/Weapons/ST_BaseWeapon.h"
#include "ST_AmmoRefillSourceComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERIOUSTANK_API UST_AmmoRefillSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool HasAvailableAmmo(TSubclassOf<AST_BaseWeapon> WeaponClass);
	int32 TakeAvailableAmmo(TSubclassOf<AST_BaseWeapon> WeaponClass, int32 RequestedAmmoAmount);

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bAreWeaponsLimited = false;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bIsAmmoLimited = false;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<TSubclassOf<AST_BaseWeapon>, int32> AmmoLimits;
};

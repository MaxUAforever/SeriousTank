#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "ST_AbilityTask_SpawnProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnProjectileDelegate, AActor*, SpawnedProjectile);

class AST_BaseProjectile;
class AST_ProjectileWeapon;

UCLASS()
class SERIOUSTANK_API UST_AbilityTask_SpawnProjectile : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSpawnProjectileDelegate OnSpawnProjectile;

private:
	AST_ProjectileWeapon* SourceWeapon;

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category="Ability|Tasks")
	static UST_AbilityTask_SpawnProjectile* SpawnProjectile(UGameplayAbility* OwningAbility);
};

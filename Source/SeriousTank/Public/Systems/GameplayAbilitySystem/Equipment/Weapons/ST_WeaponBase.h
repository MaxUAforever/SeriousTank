#pragma once

#include "Systems/GameplayAbilitySystem/Equipment/ST_EquippableAbilityItem.h"
#include "ST_WeaponBase.generated.h"

class UArrowComponent;
class USoundCue;

UCLASS()
class SERIOUSTANK_API AST_WeaponBase : public AST_EquippableAbilityItem
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoCountChanged, int32)
    FOnAmmoCountChanged OnAmmoCountChanged;
    
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ShootingArrowComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* ShootSound;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float DefaultReloadingTime;
    
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 TotalAmmoCount;

public:
	AST_WeaponBase();

	FORCEINLINE USoundCue* GetShootSound() const { return ShootSound; }
	FORCEINLINE float GetDefaultReloadingTime() const { return DefaultReloadingTime; }
	FORCEINLINE int32 GetTotalAmmoCount() const { return TotalAmmoCount; }

	const FTransform& GetMuzzleTransform() const;
};

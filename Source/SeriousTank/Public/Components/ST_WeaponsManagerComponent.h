#pragma once

#include "Components/ActorComponent.h"
#include "ST_WeaponsManagerComponent.generated.h"

class ABaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()
		
private:
	TArray<ABaseWeapon*> Weapons;
	uint32 CurrentWeaponIndex;

public:	
	UST_WeaponsManagerComponent();

	void StartFire();
	void StopFire();

	bool SwitchWeapon(int32 WeaponIndex);

protected:
	virtual void BeginPlay() override;
};

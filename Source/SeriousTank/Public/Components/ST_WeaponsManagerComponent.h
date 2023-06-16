#pragma once

#include "Components/ActorComponent.h"
#include "ST_WeaponsManagerComponent.generated.h"

class AST_BaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()
		
private:
	TArray<AST_BaseWeapon*> Weapons;
	uint32 CurrentWeaponIndex;

public:	
	UST_WeaponsManagerComponent();

	void StartFire();
	void StopFire();

	bool SwitchWeapon(int32 WeaponIndex);

	AST_BaseWeapon* GetWeapon(int32 WeaponIndex) const;

protected:
	virtual void BeginPlay() override;
};

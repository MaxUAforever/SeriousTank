#pragma once

#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "ST_VehicleWeaponManagerComponent.generated.h"

class AController;

UCLASS()
class SERIOUSTANK_API UST_VehicleWeaponManagerComponent : public UST_BaseWeaponsManagerComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};

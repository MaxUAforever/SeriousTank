#include "GameFramework/ST_GameInstance.h"

void UST_GameInstance::SetVehicle(TSubclassOf<AST_BaseVehicle> NewVehicleClass)
{
	VehicleInfo.VehicleClass = NewVehicleClass;
}

bool UST_GameInstance::SetWeapon(TSubclassOf<ABaseWeapon> NewWeaponClass, int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= VehicleInfo.WeaponClasses.Num())
	{
		return false;
	}

	VehicleInfo.WeaponClasses[WeaponIndex] = NewWeaponClass;
	return true;
}

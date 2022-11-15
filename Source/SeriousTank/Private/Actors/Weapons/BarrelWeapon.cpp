#include "Actors/Weapons/BarrelWeapon.h"

void ABarrelWeapon::StartShooting()
{
	Shoot();
	StartReloading();
}

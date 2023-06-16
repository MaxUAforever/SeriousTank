#include "Actors/Weapons/ST_BarrelWeapon.h"

void AST_BarrelWeapon::StartShooting()
{
	Shoot();
	StartReloading();
}
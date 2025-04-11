#include "Actors/Weapons/ST_BarrelWeapon.h"

void AST_BarrelWeapon::StartShooting()
{
	if (!IsCharged)
	{
		return;
	}

	IsCharged = false;
	Shoot();
}
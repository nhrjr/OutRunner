#include "stdafx.h"
#include "FlameThrower.h"
#include "Projectiles/Flame.h"
#include "Core/TextureManager.h"

static unsigned int g_seed;

//Used to seed the generator.

inline void fast_srand(int seed)
{
	g_seed = seed;
}
//fastrand routine returns one integer, similar output value range as C lib.
inline int fastrand()
{
	g_seed = (214013 * g_seed + 2531011);

	return (g_seed >> 16) & 0x7FFF;
}


FlameThrower::FlameThrower(TextureManager& t) : Weapon("FlameThrower", t)
{
	this->cooldownTime = 0.005f;
	this->reloadTime = 2.0f;
	this->ammoCap = 300;
	this->ammo = ammoCap;
	fast_srand((int)time(nullptr));
	weaponModel.setOrigin(-50, -10);
}


FlameThrower::~FlameThrower()
{
}

void FlameThrower::shoot()
{
	int spread = 40;
	float angle = weaponModel.getRotation() - spread / 2 + (fastrand() % spread);
	spawnedEntities.emplace_back(std::make_shared<Flame>(angle, getBarrelPosition()));
}
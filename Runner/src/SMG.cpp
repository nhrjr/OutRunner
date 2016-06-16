#include "stdafx.h"
#include "SMG.h"
#include "Console.h"

SMG::SMG(TextureManager& t) : Weapon("SMG", t)
{
	this->cooldownTime = 0.1f;
	this->reloadTime = 2.0f;
	this->ammoCap = 30;
	this->ammo = ammoCap;
	weaponModel.setOrigin(-47, -8.5);
}

SMG::~SMG()
{
}

void SMG::shoot()
{
	
	spawnedEntities.emplace_back(std::make_shared<Bullet>(weaponModel.getRotation(), getBarrelPosition()));
	weaponModel.spriteDefs[0].setnextState(WEAPON::SHOOT);
}




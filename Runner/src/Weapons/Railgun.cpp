#include "stdafx.h"
#include "Railgun.h"
#include "Projectiles/Beam.h"


Railgun::Railgun(TextureManager& t) : Weapon("Railgun", t)
{
	this->cooldownTime = 1.0f;
	this->reloadTime = 2.0f;
	this->ammoCap = 5;
	this->ammo = ammoCap;
	weaponModel.setOrigin(-50, -10);
}


Railgun::~Railgun()
{
}

void Railgun::shoot()
{
	spawnedEntities.emplace_back(std::make_shared<Beam>(weaponModel.getRotation(), getBarrelPosition()));
}

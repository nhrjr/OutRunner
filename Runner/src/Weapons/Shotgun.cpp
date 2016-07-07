#include "stdafx.h"
#include "Shotgun.h"

Shotgun::Shotgun(TextureManager& t) : Weapon("Shotgun",t)
{
	this->cooldownTime = 0.6f;
	this->reloadTime = 2.0f;
	this->ammoCap = 8;
	this->ammo = ammoCap;
	weaponModel.setOrigin(-50, -10);
}

Shotgun::~Shotgun()
{
}

void Shotgun::shoot()
{
	float spread = 10.0f;
	float numberOfBullets = 15;
	float nextBullet = spread / numberOfBullets;
	for (int i = 0; i < numberOfBullets; ++i)
	{
		float angle = weaponModel.getRotation() - spread / 2 + nextBullet * i;
		spawnedEntities.emplace_back(std::make_shared<Pellet>(angle, getBarrelPosition()));
		
	}
}


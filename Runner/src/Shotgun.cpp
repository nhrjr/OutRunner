#include "stdafx.h"
#include "Shotgun.h"

Shotgun::Shotgun()
{
	this->reloadTime = 0.6f;
}

Shotgun::~Shotgun()
{
}

std::vector<std::shared_ptr<Projectile>> Shotgun::shoot()
{
	this->reloadTimer = 0.0;
	this->ready = false;
	std::vector<std::shared_ptr<Projectile>> bullets;
	float spread = 10.0f;
	float numberOfBullets = 15;
	float nextBullet = spread / numberOfBullets;
	for (int i = 0; i < numberOfBullets; ++i)
	{
		float angle = weaponModel.getRotation() - spread / 2 + nextBullet * i;
		bullets.emplace_back(std::make_shared<Pellet>(angle, getPosition()));
	}

	return bullets;
}


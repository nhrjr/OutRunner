#include "stdafx.h"
#include "FlameThrower.h"
#include "Flame.h"


FlameThrower::FlameThrower()
{
	this->reloadTime = 0.05f;
}


FlameThrower::~FlameThrower()
{
}

std::vector<std::shared_ptr<Projectile>> FlameThrower::shoot()
{
	this->reloadTimer = 0.0;
	this->ready = false;
	std::vector<std::shared_ptr<Projectile>> bullets;

	float spread = 30.0f;
	float numberOfBullets = 3;
	float nextBullet = spread / numberOfBullets;
	for (int i = 0; i < numberOfBullets; ++i)
	{
		float angle = weaponModel.getRotation() - spread / 2 + nextBullet * i;
		bullets.emplace_back(std::make_shared<Flame>(angle, getPosition()));
	}

	return bullets;
}
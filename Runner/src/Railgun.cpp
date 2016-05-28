#include "stdafx.h"
#include "Railgun.h"
#include "Beam.h"


Railgun::Railgun()
{
	this->reloadTime = 1.0f;
}


Railgun::~Railgun()
{
}

std::vector<std::shared_ptr<Projectile>> Railgun::shoot()
{
	this->reloadTimer = 0.0;
	this->ready = false;
	std::vector<std::shared_ptr<Projectile>> bullets;
	bullets.emplace_back(std::make_shared<Beam>(weaponModel.getRotation(), getPosition()));
	return bullets;
}

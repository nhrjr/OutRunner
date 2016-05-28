#include "stdafx.h"
#include "SMG.h"

SMG::SMG()
{
	this->reloadTime = 0.1f;
}

SMG::~SMG()
{
}

std::vector<std::shared_ptr<Projectile>> SMG::shoot()
{
	this->reloadTimer = 0.0;
	this->ready = false;
	std::vector<std::shared_ptr<Projectile>> bullets;
	bullets.emplace_back(std::make_shared<Bullet>(weaponModel.getRotation(), getPosition()));
	return bullets;
}




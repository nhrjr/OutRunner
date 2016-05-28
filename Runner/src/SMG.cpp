#include "stdafx.h"
#include "SMG.h"

//template<typename projT>
//SMG<projT>::SMG()
SMG::SMG()
{
	this->damage = 5;
	this->reloadTime = 0.1f;
}

//template<typename projT>
//SMG<projT>::~SMG()
SMG::~SMG()
{
}

//template<typename projT>
//std::vector<std::shared_ptr<projT>> SMG<projT>::shoot()
std::vector<std::shared_ptr<Projectile>> SMG::shoot()
{
	this->reloadTimer = 0.0;
	this->ready = false;
	std::vector<std::shared_ptr<Projectile>> bullets;
	bullets.emplace_back(std::make_shared<Bullet>(weaponModel.getRotation(), getPosition()));
	return bullets;
}




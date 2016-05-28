#pragma once
#include "Weapon.h"
#include "Bullet.h"

//template<typename projT>
class SMG :
	public Weapon
{
public:
	SMG();
	~SMG();
	virtual std::vector<std::shared_ptr<Projectile>> shoot();
};



#pragma once
#include "Weapon.h"
#include "Bullet.h"

class SMG :
	public Weapon
{
public:
	SMG();
	~SMG();
	virtual std::vector<std::shared_ptr<Projectile>> shoot();
};



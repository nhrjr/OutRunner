#pragma once
#include "Weapon.h"
class FlameThrower :
	public Weapon
{
public:
	FlameThrower();
	~FlameThrower();
	virtual std::vector<std::shared_ptr<Projectile>> shoot();
};


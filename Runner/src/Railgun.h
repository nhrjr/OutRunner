#pragma once
#include "Weapon.h"
class Railgun :
	public Weapon
{
public:
	Railgun();
	~Railgun();
	virtual std::vector<std::shared_ptr<Projectile>> shoot();
};


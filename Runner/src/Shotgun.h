#pragma once
#include "Weapon.h"
#include "Pellet.h"

//template<typename projT>
class Shotgun :
	public Weapon
{
public:
	Shotgun();
	~Shotgun();
	virtual std::vector<std::shared_ptr<Projectile>> shoot();
};

#pragma once
#include "Weapon.h"
#include "Pellet.h"

//template<typename projT>
class Shotgun :
	public Weapon
{
public:
	Shotgun(TextureManager& t);
	~Shotgun();
	virtual void shoot();
};

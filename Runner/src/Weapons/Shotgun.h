#pragma once
#include "Weapon.h"
#include "Projectiles/Pellet.h"

class Shotgun :
	public Weapon
{
public:
	Shotgun(TextureManager& t);
	~Shotgun();
	virtual void shoot();
};

#pragma once
#include "Weapon.h"
#include "Projectiles/Bullet.h"

class SMG :
	public Weapon
{
public:
	SMG(TextureManager& t);
	~SMG();
	virtual void shoot();
};



#pragma once
#include "Weapon.h"
#include "Bullet.h"

class SMG :
	public Weapon
{
public:
	SMG(TextureManager& t);
	~SMG();
	virtual void shoot();
};



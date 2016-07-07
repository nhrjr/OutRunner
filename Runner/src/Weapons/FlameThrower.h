#pragma once
#include "Weapon.h"
class FlameThrower :
	public Weapon
{
public:
	FlameThrower(TextureManager& t);
	~FlameThrower();
	virtual void shoot();
};


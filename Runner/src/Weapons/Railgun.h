#pragma once
#include "Weapon.h"
class Railgun :
	public Weapon
{
public:
	Railgun(TextureManager& t);
	~Railgun();
	virtual void shoot();
};


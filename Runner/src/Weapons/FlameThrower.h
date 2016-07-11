#pragma once
#include "Weapon.h"

class TextureManager;
class FlameThrower :
	public Weapon
{
public:
	FlameThrower(TextureManager& t);
	~FlameThrower();
	virtual void shoot();
};


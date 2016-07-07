#pragma once
#include "Projectile.h"
class Bullet :
	public Projectile
{
public:
	Bullet(float angle, sf::Vector2f pos);
	~Bullet();
};


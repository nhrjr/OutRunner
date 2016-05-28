#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet(float angle, sf::Vector2f pos) : Projectile(angle, pos)
{
	this->damage = 5;
}

Bullet::~Bullet()
{
}

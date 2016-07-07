#include "stdafx.h"
#include "Pellet.h"


Pellet::Pellet(float angle, sf::Vector2f pos) : Projectile(angle, pos)
{
	this->damage = 5;
	this->speed = BULLET_SPEED * 2;
}


Pellet::~Pellet()
{
}

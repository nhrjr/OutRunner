#pragma once
#include "Projectile.h"
class Flame :
	public Projectile
{
public:
	Flame(float angle, sf::Vector2f pos);
	~Flame();

	sf::ConvexShape projectileModel;
	float rotationSpeed;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);
};


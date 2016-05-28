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

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;
	virtual void update(float dt);
};


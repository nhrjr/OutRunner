#pragma once
#include "Projectile.h"
class Beam :
	public Projectile
{
public:
	Beam(float angle, sf::Vector2f pos);
	~Beam();

	sf::RectangleShape projectileModel;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//virtual void draw(sf::RenderWindow& window);
	//virtual void setPosition(sf::Vector2f pos);
	//virtual sf::Vector2f getPosition() const;
	virtual void update(float dt);
};


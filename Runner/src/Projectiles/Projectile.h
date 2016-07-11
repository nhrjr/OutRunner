#pragma once
#include "GameEntities/IAtomicEntity.h"
#include "Core/Game.h"

class Projectile :
	public IAtomicEntity
{
public:
	Projectile(float angle, sf::Vector2f pos, unsigned int pointCount = 2);
	virtual ~Projectile() = 0;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);
	void collide(IAtomicEntity& other, unsigned int type, float dt);

	sf::Vector2f getDirection() const;

protected:
	sf::CircleShape projectileModel;
	sf::Vector2f direction;
	float lifeTime;
	float speed;

	virtual float getMinDistance() const;
	
};


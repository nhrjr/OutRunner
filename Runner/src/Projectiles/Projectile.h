#pragma once
#include "GameEntities/IAtomicEntity.h"
#include "Core/Game.h"

class Projectile :
	public IAtomicEntity
{
public:
	Projectile(float angle, sf::Vector2f pos, unsigned int pointCount = 2);
	virtual ~Projectile() = 0;

	//virtual void draw(sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//virtual void setPosition(sf::Vector2f pos);
	//virtual sf::Vector2f getPosition() const ;
	virtual void update(float dt);
	void collide(IAtomicEntity& other, unsigned int type, float dt);

	sf::Vector2f getDirection() const;

protected:
	sf::CircleShape projectileModel;
	sf::Vector2f direction;
	float lifeTime;
	float speed;

	//virtual sf::Vector2f getPoint(int i) const;
	//virtual unsigned int getPointCount() const;
	//virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
	
};


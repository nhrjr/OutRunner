#pragma once
#include "IGameEntity.h"
#include "Game.h"

class Projectile :
	public IGameEntity
{
public:
	Projectile(float angle, sf::Vector2f pos);
	virtual ~Projectile() = 0;

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const ;
	virtual void update(float dt);
	void collide(IGameEntity& other, unsigned int type, float dt);

protected:
	sf::CircleShape projectileModel;
	sf::Vector2f direction;
	float lifeTime;
	float speed;

	virtual sf::Vector2f getPoint(int i) const;
	virtual unsigned int getPointCount() const;
	virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


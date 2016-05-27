#pragma once
#include "IGameEntity.h"
#include "Game.h"
class Bullet :
	public IGameEntity
{
public:
	Bullet(float angle, sf::Vector2f pos);
	~Bullet();

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const ;
	virtual void update(float dt);
	void collide(IGameEntity& other, unsigned int type, float dt);

	//void updateCollision(sf::Vector2f displace, float dt);

private:
	sf::CircleShape bulletModel;
	sf::Vector2f direction;
	float lifeTime;
	float damage;

	virtual sf::Vector2f getPoint(int i) const;
	virtual int getPointCount() const;
	virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


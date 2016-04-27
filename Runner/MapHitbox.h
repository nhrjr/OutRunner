#pragma once
#include "IGameEntity.h"
#include "Game.h"
class MapHitbox :
	public IGameEntity
{
public:
	MapHitbox(sf::Vector2f size);
	MapHitbox();
	~MapHitbox();

	virtual void draw(sf::RenderWindow& window, float dt);
	virtual void update(float dt);

	virtual void setPosition(sf::Vector2f pos);
	
	virtual sf::Vector2f getPosition() const;
	virtual sf::Vector2f getPoint(int i) const;
	virtual int getPointCount() const;
	virtual sf::Transform getTransform() const;
};


#pragma once
#include "GameEntities/IGameEntity.h"
#include "Core/Game.h"
class MapHitbox :
	public IGameEntity
{
public:
	MapHitbox(sf::Vector2f size);
	MapHitbox();
	~MapHitbox();

	virtual void draw(sf::RenderWindow& window);
	virtual void update(float dt);

	virtual void setPosition(sf::Vector2f pos);
	
	virtual sf::Vector2f getPosition() const;
	virtual sf::Vector2f getPoint(int i) const;
	virtual unsigned int getPointCount() const;
	virtual sf::Transform getTransform() const;
};


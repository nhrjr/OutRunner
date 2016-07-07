#pragma once
#include "IAtomicEntity.h"
#include "Game.h"
class MapHitbox :
	public IAtomicEntity
{
public:
	MapHitbox(sf::Vector2f size);
	MapHitbox();
	~MapHitbox();

	//void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);

	//virtual void setPosition(sf::Vector2f pos);
	
	//virtual sf::Vector2f getPosition() const;
	//virtual sf::Vector2f getPoint(int i) const;
	//virtual unsigned int getPointCount() const;
	//virtual sf::Transform getTransform() const;
};


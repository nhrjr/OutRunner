#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class IGameEntity 
{
public:
		
	sf::Vector2f position;
	bool shouldEnd = false;
	sf::ConvexShape hitbox;
	int hitpoints;

	virtual ~IGameEntity() {};

	virtual void collide(IGameEntity& other, unsigned int type, float dt) {};

	virtual void draw(sf::RenderWindow& window, float dt) = 0;
	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual sf::Vector2f getPosition() const = 0 ;
	virtual void update(float dt) = 0;

	virtual sf::Vector2f getPoint(int i) const { return sf::Vector2f(0, 0); } ;
	virtual int getPointCount() const { return 0; } ;
	virtual sf::Transform getTransform() const { return sf::Transform(); };
	virtual float getMinDistance() const { return 0; };
};
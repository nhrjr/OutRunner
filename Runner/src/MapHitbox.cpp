#include "stdafx.h"
#include "MapHitbox.h"
#define POINTCOUNT 4

MapHitbox::MapHitbox(sf::Vector2f size)
{
	hitbox = sf::ConvexShape(POINTCOUNT);
	hitbox.setPoint(0, sf::Vector2f(0,0));
	hitbox.setPoint(1, sf::Vector2f(0, size.y));
	hitbox.setPoint(2, size);
	hitbox.setPoint(3, sf::Vector2f(size.x, 0));
	
	hitbox.setOutlineThickness(-2);
	hitbox.setOutlineColor(sf::Color::Black);
	hitbox.setFillColor(sf::Color(255, 255, 255, 128));
}

MapHitbox ::MapHitbox()
{

}


MapHitbox::~MapHitbox()
{
}

void MapHitbox::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
}

void MapHitbox::update(float dt)
{

}

void MapHitbox::setPosition(sf::Vector2f pos)
{
	hitbox.setPosition(pos);
}

sf::Vector2f MapHitbox::getPosition() const
{
	return hitbox.getPosition();
}

sf::Vector2f MapHitbox::getPoint(int i) const
{
	return hitbox.getPoint(i);
}
unsigned int MapHitbox::getPointCount() const
{
	return hitbox.getPointCount();
}
sf::Transform MapHitbox::getTransform() const
{
	return hitbox.getTransform();
}
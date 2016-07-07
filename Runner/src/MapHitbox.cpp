#include "stdafx.h"
#include "MapHitbox.h"
#define POINTCOUNT 4

MapHitbox::MapHitbox(sf::Vector2f size) : IAtomicEntity(POINTCOUNT)
{
	//hitbox = sf::ConvexShape(POINTCOUNT);
	this->setPoint(0, sf::Vector2f(0,0));
	this->setPoint(1, sf::Vector2f(0, size.y));
	this->setPoint(2, size);
	this->setPoint(3, sf::Vector2f(size.x, 0));
	
	this->setOutlineThickness(-2);
	this->setOutlineColor(sf::Color::Black);
	this->setFillColor(sf::Color(255, 255, 255, 128));
}

MapHitbox ::MapHitbox()
{

}


MapHitbox::~MapHitbox()
{
}

//void MapHitbox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
//	states.transform *= this->getTransform();
//	//target.draw(projectileModel, states);
//}

void MapHitbox::update(float dt)
{

}

//void MapHitbox::setPosition(sf::Vector2f pos)
//{
//	hitbox.setPosition(pos);
//}
//
//sf::Vector2f MapHitbox::getPosition() const
//{
//	return hitbox.getPosition();
//}
//
//sf::Vector2f MapHitbox::getPoint(int i) const
//{
//	return hitbox.getPoint(i);
//}
//unsigned int MapHitbox::getPointCount() const
//{
//	return hitbox.getPointCount();
//}
//sf::Transform MapHitbox::getTransform() const
//{
//	return hitbox.getTransform();
//}
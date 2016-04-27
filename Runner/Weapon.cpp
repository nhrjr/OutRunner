#include "stdafx.h"
#include "Weapon.h"


Weapon::Weapon() : weaponModel(sf::Vector2f(20, 10)), reloadTime(0.1), reloadTimer(0)
{
	weaponModel.setFillColor(sf::Color::Red);
	//weaponModel.setOrigin(-5, -10);
	weaponModel.setOrigin(0, 5);
}


Weapon::~Weapon()
{
}

void Weapon::setPosition(sf::Vector2f pos)
{
	weaponModel.setPosition(pos);
}

sf::Vector2f Weapon::getPosition() const
{
	return weaponModel.getPosition();
}

void Weapon::draw(sf::RenderWindow& window, float dt)
{
	window.draw(weaponModel);
}

void Weapon::attachedMove(sf::Vector2f directionOffset, float angleOffset)
{
	this->weaponModel.setRotation(angleOffset);
	this->weaponModel.move(directionOffset);
}

void Weapon::update(float dt)
{
	
}
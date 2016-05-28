#include "stdafx.h"
#include "Weapon.h"


//template<typename projT>
//Weapon<projT>::Weapon() : weaponModel(sf::Vector2f(20, 10)), reloadTimer(0)
Weapon::Weapon() : weaponModel(sf::Vector2f(20, 10)), reloadTimer(0)
{
	weaponModel.setFillColor(sf::Color::Red);
	//weaponModel.setOrigin(-5, -10);
	weaponModel.setOrigin(0, 5);
}

//template<typename projT>
//Weapon<projT>::~Weapon()
Weapon::~Weapon()
{
}

//template<typename projT>
//void Weapon<projT>::setPosition(sf::Vector2f pos)
void Weapon::setPosition(sf::Vector2f pos)
{
	weaponModel.setPosition(pos);
}

//template<typename projT>
//sf::Vector2f Weapon<projT>::getPosition() const
sf::Vector2f Weapon::getPosition() const
{
	return weaponModel.getPosition();
}

//template<typename projT>
//void Weapon<projT>::draw(sf::RenderWindow& window)
void Weapon::draw(sf::RenderWindow& window)
{
	if (this->triggered)
		window.draw(weaponModel);
}

//template<typename projT>
//void Weapon<projT>::attachedMove(sf::Vector2f directionOffset, float angleOffset)
void Weapon::attachedMove(sf::Vector2f directionOffset, float angleOffset)
{
	this->weaponModel.setRotation(angleOffset);
	this->weaponModel.move(directionOffset);
}

//template<typename projT>
//void Weapon<projT>::attachedSetPosition(sf::Vector2f newPos, float angleOffset)
void Weapon::attachedSetPosition(sf::Vector2f newPos, float angleOffset)
{
	this->weaponModel.setRotation(angleOffset);
	this->weaponModel.setPosition(newPos);
}

//template<typename projT>
//void Weapon<projT>::update(float dt)
void Weapon::update(float dt)
{

	if (reloadTimer > reloadTime)
	{
		ready = true;
	}
	else
	{
		this->reloadTimer += dt;
	}
}

//template<typename projT>
//void Weapon<projT>::toggleTrigger()
void Weapon::toggleTrigger()
{
	this->triggered = (triggered == true) ? false : true;
}

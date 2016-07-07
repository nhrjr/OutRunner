#include "stdafx.h"
#include "Beam.h"
#include "Console.h"

#include <cmath>

Beam::Beam(float angle, sf::Vector2f pos) : Projectile(angle, pos)
{
	damage = 55;
	//hitbox = sf::ConvexShape(2);
	this->setPoint(0, sf::Vector2f(0, 1));
	this->setPoint(1, sf::Vector2f(2, 1));
	this->setOrigin(1, 0.5);
	this->setRotation(angle);

	projectileModel.setSize(sf::Vector2f(2, 2));
	projectileModel.setFillColor(sf::Color::Green);

	//projectileModel.setOrigin(1, 1);
	//projectileModel.setRotation(angle);

	direction.x = cos(angle * M_PI / 180);
	direction.y = sin(angle * M_PI / 180);

	this->setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
	//projectileModel.setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
}


Beam::~Beam()
{
}

//void Beam::draw(sf::RenderWindow& window)
//{
//	window.draw(projectileModel);
//}

//void Beam::setPosition(sf::Vector2f pos)
//{
//	projectileModel.setPosition(pos);
//	hitbox.setPosition(pos);
//}

//sf::Vector2f Beam::getPosition() const
//{
//	return projectileModel.getPosition();
//}

void Beam::update(float dt)
{
	if (this->isDeletable == false)
	{
		lifeTime += dt;
		sf::Vector2f forward = direction * BULLET_SPEED * dt;
		
		//projectileModel.move(forward * 3.0f);
		this->move(forward * 3.0f);

		if (direction != sf::Vector2f(0, 0))
		{
			projectileModel.setSize(sf::Vector2f(-BULLET_SPEED * lifeTime, 2));
			this->setPoint(1,sf::Vector2f(-BULLET_SPEED * lifeTime, 1));
		}
		else
		{
			sf::Vector2f size = projectileModel.getSize();
			if (size.x < 0)
			{
				projectileModel.setSize(sf::Vector2f(size.x + BULLET_SPEED * dt, size.y));
				this->setPoint(1, sf::Vector2f(size.x + BULLET_SPEED * dt, 1));
			}
			
		}
		
		
		if (lifeTime > 0.5f)
			this->isDeletable = true;
	}
}

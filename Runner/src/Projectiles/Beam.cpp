#include "stdafx.h"
#include "Projectiles/Beam.h"
#include "Core/Console.h"

#include <cmath>

Beam::Beam(float angle, sf::Vector2f pos) : Projectile(angle, pos)
{
	damage = 55;
	this->setPoint(0, sf::Vector2f(0, 1));
	this->setPoint(1, sf::Vector2f(2, 1));
	this->setOrigin(1, 0.5);
	this->setRotation(angle);

	projectileModel.setSize(sf::Vector2f(2, 2));
	projectileModel.setFillColor(sf::Color::Green);

	direction.x = cos(angle * M_PI / 180);
	direction.y = sin(angle * M_PI / 180);

	this->setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
}


Beam::~Beam()
{
}

void Beam::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	target.draw(projectileModel, states);
}

void Beam::update(float dt)
{
	if (this->isDeletable == false)
	{
		lifeTime += dt;
		sf::Vector2f forward = direction * BULLET_SPEED * dt;
		
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

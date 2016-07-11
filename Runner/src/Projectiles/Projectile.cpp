#include "stdafx.h"
#include "Projectile.h"
#include <memory>
#include "Algorithms/SAT.h"
#include <cmath>


Projectile::Projectile(float angle, sf::Vector2f pos, unsigned int pointCount) : IAtomicEntity(pointCount), projectileModel(4), lifeTime(0.0f), speed(BULLET_SPEED)
{
	this->setPoint(0, sf::Vector2f(0, 2));
	this->setPoint(1, sf::Vector2f(8, 2));

	this->setOrigin(4, 2);
	this->setRotation(angle);

	projectileModel.setFillColor(sf::Color::Black);
	projectileModel.setScale(1, 0.5);
	

	direction.x = cos(angle * M_PI / 180);
	direction.y = sin(angle * M_PI / 180);

	this->setPosition(pos +  sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
}

Projectile::~Projectile()
{
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	target.draw(projectileModel, states);
}

void Projectile::update(float dt)
{
	if (this->isDeletable == false)
	{
		sf::Vector2f forward = direction * speed * dt;
		this->move(forward);
		lifeTime += dt;
		if (lifeTime > 1.0f)
			this->isDeletable = true;
	}
}

void Projectile::collide(IAtomicEntity& other, unsigned int type, float dt)
{
	sf::Vector2f displace = Collision::SATCollision(this, &other);
	if (displace != sf::Vector2f(0, 0))
	{
		if (type == 0)
		{
			this->direction = sf::Vector2f(0, 0);
		}
	}
}

float Projectile::getMinDistance() const
{
	return 2;
}

sf::Vector2f Projectile::getDirection() const
{
	return direction;
}


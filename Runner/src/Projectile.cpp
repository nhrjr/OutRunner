#include "stdafx.h"
#include "Projectile.h"
#include <memory>
#include "SAT.h"
#include <cmath>


Projectile::Projectile(float angle, sf::Vector2f pos) : projectileModel(4), lifeTime(0.0f)
{
	hitbox = sf::ConvexShape(2);
	hitbox.setPoint(0, sf::Vector2f(0, 2));
	hitbox.setPoint(1, sf::Vector2f(8, 2));

	hitbox.setOrigin(4, 2);
	hitbox.setRotation(angle);

	projectileModel.setFillColor(sf::Color::Black);
	projectileModel.setScale(1, 0.5);
	projectileModel.setOrigin(4, 4);
	projectileModel.setRotation(angle);
	

	direction.x = cos(angle * M_PI / 180);
	direction.y = sin(angle * M_PI / 180);

	//hitbox.setPosition(pos + 15.0f * sf::Vector2f(-direction.y, direction.x) + 20.0f * direction);
	//projectileModel.setPosition(pos + 15.0f * sf::Vector2f(-direction.y, direction.x) + 20.0f * direction);
	hitbox.setPosition(pos +  sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
	projectileModel.setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
}

Projectile::~Projectile()
{
}

void Projectile::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
	window.draw(projectileModel);
	
}

void Projectile::setPosition(sf::Vector2f pos)
{
	projectileModel.setPosition(pos);
	hitbox.setPosition(pos);
}

sf::Vector2f Projectile::getPosition() const
{
	return projectileModel.getPosition();
}

void Projectile::update(float dt)
{
	if (this->isDeletable == false)
	{
		sf::Vector2f forward = direction * BULLET_SPEED * dt;
		projectileModel.move(forward);
		hitbox.move(forward);
		lifeTime += dt;
		if (lifeTime > 1.0f)
			this->isDeletable = true;
	}
}

void Projectile::collide(IGameEntity& other, unsigned int type, float dt)
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

sf::Vector2f Projectile::getPoint(int i) const
{
	return hitbox.getPoint(i);
}

int Projectile::getPointCount() const
{
	return hitbox.getPointCount();
}
sf::Transform Projectile::getTransform() const
{
	return hitbox.getTransform();
}

float Projectile::getMinDistance() const
{
	return 2;
}


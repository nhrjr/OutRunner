#include "stdafx.h"
#include "Flame.h"

#include <cmath>


Flame::Flame(float angle, sf::Vector2f pos) : Projectile(angle, pos), projectileModel(3)
{
	this->damage = 1;
	this->rotationSpeed = 500.0f;
	hitbox = sf::ConvexShape(3);
	hitbox.setPoint(0, sf::Vector2f(12, 0));
	hitbox.setPoint(1, sf::Vector2f(12, 12));
	hitbox.setPoint(2, sf::Vector2f(0,6));
	hitbox.setOrigin(6, 6);
	hitbox.setRotation(angle);

	//projectileModel.setSize(sf::Vector2f(2, 2));
	projectileModel.setPoint(0, sf::Vector2f(12, 0));
	projectileModel.setPoint(1, sf::Vector2f(12, 12));
	projectileModel.setPoint(2, sf::Vector2f(0, 6));
	projectileModel.setFillColor(sf::Color::Red);

	projectileModel.setOrigin(6, 6);
	projectileModel.setRotation(angle);

	direction.x = cos(angle * M_PI / 180);
	direction.y = sin(angle * M_PI / 180);

	hitbox.setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * 3.0f * direction);
	projectileModel.setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * 3.0f * direction);
}


Flame::~Flame()
{
}

void Flame::draw(sf::RenderWindow& window)
{
	window.draw(projectileModel);
}

void Flame::setPosition(sf::Vector2f pos)
{
	projectileModel.setPosition(pos);
	hitbox.setPosition(pos);
}

sf::Vector2f Flame::getPosition() const
{
	return projectileModel.getPosition();
}

void Flame::update(float dt)
{
	if (this->isDeletable == false)
	{
		lifeTime += dt;
		sf::Vector2f forward = direction * BULLET_SPEED * 0.4f * dt;

		projectileModel.move(forward);
		hitbox.move(forward);

		projectileModel.rotate(rotationSpeed * dt);
		hitbox.rotate(rotationSpeed * dt);

		if (lifeTime > 0.4f)
			this->isDeletable = true;
	}
}
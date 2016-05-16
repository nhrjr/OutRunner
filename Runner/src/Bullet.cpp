#include "stdafx.h"
#include "Bullet.h"
#include <memory>
#include "SAT.h"
#include <cmath>


Bullet::Bullet(Game* game,float angle, sf::Vector2f pos) : game(game), bulletModel(4), lifeTime(0.0f)
{
	hitbox = sf::ConvexShape(2);
	hitbox.setPoint(0, sf::Vector2f(8, 2));
	hitbox.setPoint(1, sf::Vector2f(0, 2));
	//hitbox.setPoint(1, sf::Vector2f(0, 0));
	//hitbox.setPoint(2, sf::Vector2f(0, 4));
	//hitbox.setFillColor(sf::Color::White);
	hitbox.setOrigin(4, 2);
	hitbox.setRotation(angle);

	bulletModel.setFillColor(sf::Color::Black);
	bulletModel.setScale(1, 0.5);
	bulletModel.setOrigin(4, 4);
	bulletModel.setRotation(angle);
	

	direction.x = cos(angle * M_PI / 180);
	direction.y = sin(angle * M_PI / 180);

	//hitbox.setPosition(pos + 15.0f * sf::Vector2f(-direction.y, direction.x) + 20.0f * direction);
	//bulletModel.setPosition(pos + 15.0f * sf::Vector2f(-direction.y, direction.x) + 20.0f * direction);
	hitbox.setPosition(pos +  sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
	bulletModel.setPosition(pos + sf::Vector2f(-direction.y, direction.x) + PLAYER_RADIUS * direction);
}


Bullet::~Bullet()
{
}


void Bullet::draw(sf::RenderWindow& window, float dt)
{
	//window.draw(hitbox);
	window.draw(bulletModel);
	
}

void Bullet::setPosition(sf::Vector2f pos)
{
	bulletModel.setPosition(pos);
	hitbox.setPosition(pos);
}

sf::Vector2f Bullet::getPosition() const
{
	return bulletModel.getPosition();
}

void Bullet::update(float dt)
{
	if (this->shouldEnd == false)
	{
		sf::Vector2f forward = direction * BULLET_SPEED * dt;
		bulletModel.move(forward);
		hitbox.move(forward);
		lifeTime += dt;
		if (lifeTime > 1.0f)
			this->shouldEnd = true;
	}
}

//void Bullet::updateCollision(sf::Vector2f displace, float dt)
//{
//	if (displace != sf::Vector2f(0, 0))
//		//this->shouldEnd = true;
//		this->direction = sf::Vector2f(0, 0);
//		//this->lifeTime = 0.96f;
//		//this->direction += displace;
//	//{
//	//	float moveBy = BULLET_SPEED * dt;
//
//	//	this->hitbox->move(displace * moveBy);
//	//	this->bulletModel.move(displace * moveBy);
//	//}
//	
//}

void Bullet::collide(IGameEntity& other, unsigned int type, float dt)
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

sf::Vector2f Bullet::getPoint(int i) const
{
	return hitbox.getPoint(i);
}

int Bullet::getPointCount() const
{
	return hitbox.getPointCount();
}
sf::Transform Bullet::getTransform() const
{
	return hitbox.getTransform();
}

float Bullet::getMinDistance() const
{
	return 2;
}


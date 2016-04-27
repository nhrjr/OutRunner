#include "stdafx.h"
#include "Player.h"
#include "SAT.h"
#include "MapTools.h"
#include "math.h"
#include "V2Tools.h"
#include <string>
#include <memory>


Player::Player(Game* game, IPlayerInput* playerInput) : game(game), playerInput(playerInput), 
	playerModel(PLAYER_RADIUS), playerHead(PLAYER_RADIUS / 2), isAttacking(false), hitboxRadius(PLAYER_RADIUS)
{
	hitpoints = 100;

	hitbox = sf::ConvexShape(4);
	hitbox.setPoint(0, sf::Vector2f(0, 0));
	hitbox.setPoint(1, sf::Vector2f(0, PLAYER_RADIUS ));
	hitbox.setPoint(2, sf::Vector2f(PLAYER_RADIUS , PLAYER_RADIUS ));
	hitbox.setPoint(3, sf::Vector2f(PLAYER_RADIUS , 0));
	hitbox.setOutlineColor(sf::Color::White);
	hitbox.setOrigin(PLAYER_RADIUS/2, PLAYER_RADIUS / 2);
	hitbox.setRotation(45);

	//hitbox = sf::ConvexShape(1);
	//hitbox.setPoint(0, sf::Vector2f(0, 0));

	playerModel.setFillColor(sf::Color::Blue);
	playerModel.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
	playerModel.setScale(0.5, 1);

	playerHead.setFillColor(sf::Color::Black);
	playerHead.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	healthBarFull.setSize(sf::Vector2f(50, 10));
	healthBarFull.setOutlineColor(sf::Color::Black);
	healthBarFull.setFillColor(sf::Color::Green);
	healthBarFull.setOutlineThickness(-2.0f);
	healthBarFull.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS-10));

	healthBarEmpty.setSize(sf::Vector2f(50, 10));
	healthBarEmpty.setOutlineColor(sf::Color::Black);
	healthBarEmpty.setFillColor(sf::Color::Red);
	healthBarEmpty.setOutlineThickness(-2.0f);
	healthBarEmpty.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS - 10));
}


Player::~Player()
{
	delete playerInput;
}

void Player::draw(sf::RenderWindow& window, float dt)
{
	if (isAttacking)
		weapon.draw(window, dt);
	window.draw(playerModel);
	window.draw(playerHead);
	//window.draw(displacement);
	//window.draw(hitbox);

	window.draw(healthBarEmpty);
	window.draw(healthBarFull);
}

void Player::setPosition(sf::Vector2f pos)
{
	hitbox.setPosition(pos);

	playerModel.setPosition(pos);
	playerHead.setPosition(pos);

	weapon.setPosition(pos);

	healthBarEmpty.setPosition(pos);
	healthBarFull.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
	return playerModel.getPosition();
}

void Player::update(float dt)
{
	if (hitpoints <= 0) this->shouldEnd = true;

	float moveBy = PLAYER_SPEED * dt;

	float angleOffset = playerInput->getAngle();
	this->hitbox.setRotation(angleOffset+45);
	this->playerModel.setRotation(angleOffset);
	this->playerHead.setRotation(angleOffset);
	

	sf::Vector2f directionOffset = playerInput->getOffset() * moveBy;
	this->hitbox.move(directionOffset);
	this->playerModel.move(directionOffset);
	this->playerHead.move(directionOffset);

	this->healthBarEmpty.move(directionOffset);
	this->healthBarFull.move(directionOffset);


	//sf::Vector2f weaponToCursor = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window))) - weapon.getPosition();
	//float weaponToCursorAngle = atan2(weaponToCursor.y, weaponToCursor.x) * 180 / M_PI;
	//this->attackingAngle = fmod(weaponToCursorAngle, 360);
	this->attackingAngle = angleOffset;
	weapon.attachedMove(directionOffset, angleOffset);

	

	
	if (this->playerInput->getAction() > 0)
	{
		isAttacking = true;
		if (weapon.reloadTimer >= weapon.reloadTime)
		{
			weapon.reloadTimer = 0.0f;
			isShooting = true;
		}
		else
		{
			isShooting = false;
		}
			
	}
	else
	{
		isAttacking = false;
		isShooting = false;
	}
	weapon.reloadTimer += dt;


	if (this->playerInput->getAlternateAction() > 0 && this->playerInput->getAction() == 0)
	{
		healingTimer += dt;
		if (healingTimer >= 1.0f && hitpoints < 100)
		{
			hitpoints += 5.0f;
			healingTimer = 0;
			std::cout << hitpoints << std::endl;
		}
	}
	else if (this->playerInput->getAlternateAction() == 0)
	{
		healingTimer = 0;
	}
	this->healthBarFull.setSize(sf::Vector2f(50.0f * this->hitpoints / 100.0f, 10));
}

//void Player::updateCollision(sf::Vector2f displace, float dt)
//{
//	if (displace == sf::Vector2f(0, 0)) return;
//	float moveBy = PLAYER_SPEED * dt;
//
//	float angle = atan2(displace.y, displace.x) * 180 / M_PI;
//	float length = V2Tools::length(displace);
//
//	this->hitbox.move(displace * moveBy);
//	this->playerModel.move(displace * moveBy);
//	this->playerHead.move(displace * moveBy);
//	this->weapon.attachedMove(displace * moveBy, playerInput->getAngle());
//}

void Player::collide(IGameEntity& other, unsigned int type, float dt)
{
	sf::Vector2f displace = Collision::SATCollision(this, &other);
	if (displace != sf::Vector2f(0, 0))
	{
		if (type == 1)
		{
			this->hitpoints -= 5;
			other.shouldEnd = true;
		}
		if (type == 0)
		{
			float moveBy = PLAYER_SPEED * dt;

			float angle = atan2(displace.y, displace.x) * 180 / M_PI;
			float length = V2Tools::length(displace);

			this->hitbox.move(displace * moveBy);
			this->playerModel.move(displace * moveBy);
			this->playerHead.move(displace * moveBy);
			this->weapon.attachedMove(displace * moveBy, playerInput->getAngle());

			this->healthBarEmpty.move(displace * moveBy);
			this->healthBarFull.move(displace * moveBy);
		}
	}
}



sf::Vector2f Player::getPoint(int i) const
{
	return hitbox.getPoint(i);
}
int Player::getPointCount() const
{
	return hitbox.getPointCount();
}
sf::Transform Player::getTransform() const
{
	return hitbox.getTransform();
}

float Player::getMinDistance() const
{
	return PLAYER_RADIUS;
}

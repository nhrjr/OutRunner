#include "stdafx.h"
#include "Player.h"
#include "SAT.h"
#include "MapTools.h"
#include "math.h"
#include "V2Tools.h"
#include "Console.h"
#include <string>
#include <memory>

#include "SMG.h"
#include "Shotgun.h"
#include "Railgun.h"
#include "FlameThrower.h"


Player::Player()
{
}

Player::Player(Game* game, IPlayerInput* playerInput) : game(game), playerInput(playerInput), 
	playerModel(PLAYER_RADIUS), playerHead(PLAYER_RADIUS / 2), hitboxRadius(PLAYER_RADIUS)
{
	hitpoints = 100;
	this->weapons.emplace_back(std::make_shared<Shotgun>());
	this->weapons.emplace_back(std::make_shared<SMG>());
	this->weapons.emplace_back(std::make_shared<Railgun>());
	this->weapons.emplace_back(std::make_shared<FlameThrower>());

	weapon = weapons.begin();

	hitbox = sf::ConvexShape(4);
	hitbox.setPoint(0, sf::Vector2f(0, 0));
	hitbox.setPoint(1, sf::Vector2f(0, PLAYER_RADIUS ));
	hitbox.setPoint(2, sf::Vector2f(PLAYER_RADIUS , PLAYER_RADIUS ));
	hitbox.setPoint(3, sf::Vector2f(PLAYER_RADIUS , 0));
	hitbox.setOutlineColor(sf::Color::White);
	hitbox.setOrigin(PLAYER_RADIUS/2, PLAYER_RADIUS / 2);
	hitbox.setRotation(45);

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

	//GuidGenerator gen;
	//this->entityID = gen.newGuid();
	NetworkPlayerEvent event;
	event.entityID = this->entityID;
	Console::Instance() << "Created player with " << entityID << std::endl;
	if (this->game->networkmgr.type == "client")
	{
		this->game->networkmgr.sendInitAlive(entityID);
	}
	remote = false;
	
}

Player::Player(Game* game, IPlayerInput* playerInput,const Guid& guid) : game(game), playerInput(playerInput),
playerModel(PLAYER_RADIUS), playerHead(PLAYER_RADIUS / 2), hitboxRadius(PLAYER_RADIUS)
{
	hitpoints = 100;
	this->weapons.emplace_back(std::make_shared<Shotgun>());
	this->weapons.emplace_back(std::make_shared<SMG>());
	this->weapons.emplace_back(std::make_shared<Railgun>());
	this->weapons.emplace_back(std::make_shared<FlameThrower>());
	weapon = weapons.begin();

	hitbox = sf::ConvexShape(4);
	hitbox.setPoint(0, sf::Vector2f(0, 0));
	hitbox.setPoint(1, sf::Vector2f(0, PLAYER_RADIUS));
	hitbox.setPoint(2, sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS));
	hitbox.setPoint(3, sf::Vector2f(PLAYER_RADIUS, 0));
	hitbox.setOutlineColor(sf::Color::White);
	hitbox.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);
	hitbox.setRotation(45);

	playerModel.setFillColor(sf::Color::Green);
	playerModel.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
	playerModel.setScale(0.5, 1);

	playerHead.setFillColor(sf::Color::Black);
	playerHead.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	healthBarFull.setSize(sf::Vector2f(50, 10));
	healthBarFull.setOutlineColor(sf::Color::Black);
	healthBarFull.setFillColor(sf::Color::Green);
	healthBarFull.setOutlineThickness(-2.0f);
	healthBarFull.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS - 10));

	healthBarEmpty.setSize(sf::Vector2f(50, 10));
	healthBarEmpty.setOutlineColor(sf::Color::Black);
	healthBarEmpty.setFillColor(sf::Color::Red);
	healthBarEmpty.setOutlineThickness(-2.0f);
	healthBarEmpty.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS - 10));

	this->entityID = guid;

	Console::Instance() << "Created remote Player with " << entityID << std::endl;
	remote = true;
}


Player::~Player()
{
}

void Player::draw(sf::RenderWindow& window)
{
	(*weapon)->draw(window);
	//weapon->draw(window);
	window.draw(playerModel);
	window.draw(playerHead);

	window.draw(healthBarEmpty);
	window.draw(healthBarFull);
}

void Player::setPosition(sf::Vector2f pos)
{
	hitbox.setPosition(pos);

	playerModel.setPosition(pos);
	playerHead.setPosition(pos);

	(*weapon)->setPosition(pos);

	healthBarEmpty.setPosition(pos);
	healthBarFull.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
	return playerModel.getPosition();
}

void Player::update(float dt)
{
	if (hitpoints <= 0) this->isDead = true;

	if (remote == true)
	{
		std::stack<NetworkPlayerEvent> events;
		events = this->game->networkmgr.getEvents(entityID);
		if (events.size() != 0)
		{
			hitpoints = events.top().hitpoints;
			weapon = weapons.begin() + events.top().equippedWeapon;
		}
		playerInput->getInput(dt, events);
		
	}
	else
	{
		playerInput->getInput(dt);
	}

	
	float angleOffset = playerInput->getAngle();
	sf::Vector2f newPos = playerInput->getPosition(this->getPosition());

	this->hitbox.setRotation(angleOffset+45);
	this->playerModel.setRotation(angleOffset);
	this->playerHead.setRotation(angleOffset);
		
	this->hitbox.setPosition(newPos);
	this->playerModel.setPosition(newPos);
	this->playerHead.setPosition(newPos);

	this->healthBarEmpty.setPosition(newPos);
	this->healthBarFull.setPosition(newPos);
	
	this->attackingAngle = angleOffset;

	(*weapon)->attachedSetPosition(newPos, angleOffset);
	(*weapon)->update(dt);
	(*weapon)->triggered = (this->playerInput->getAction() > 0) ? true : false;


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

	if (remote == false)
	{
		if (this->game->networkmgr.networkGameObjects.size() != 0)
		{
			NetworkPlayerEvent event;
			event.x = newPos.x;
			event.y = newPos.y;
			event.angle = playerInput->getAngle();
			event.hitpoints = hitpoints;
			event.action = playerInput->getAction();
			event.alternateAction = playerInput->getAlternateAction();
			event.equippedWeapon = weapon - weapons.begin();
			event.entityID = this->entityID;
			
			this->game->networkmgr.queueEvent(event);
		}
	}
}

void Player::collide(IGameEntity& other, unsigned int type, float dt)
{
	sf::Vector2f displace = Collision::SATCollision(this, &other);
	if (displace != sf::Vector2f(0, 0))
	{
		if (type == 1)
		{
			this->hitpoints -= other.damage;
			other.isDeletable = true;
		}
		if (type == 0)
		{
			float moveBy = PLAYER_SPEED * dt;

			float angle = atan2(displace.y, displace.x) * 180 / M_PI;
			float length = V2Tools::length(displace);

			this->hitbox.move(displace * moveBy);
			this->playerModel.move(displace * moveBy);
			this->playerHead.move(displace * moveBy);
			(*weapon)->attachedMove(displace * moveBy, playerInput->getAngle());

			this->healthBarEmpty.move(displace * moveBy);
			this->healthBarFull.move(displace * moveBy);
		}
	}
}

void Player::nextWeapon()
{
	if(weapon != --(weapons.end()))
		weapon = std::next(weapon);
}

void Player::prevWeapon()
{
	if(weapon != weapons.begin())
		weapon = std::prev(weapon);
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

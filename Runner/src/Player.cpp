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

namespace FEET
{
	enum FEET {
		IDLE,
		RUN,
		STRAFE_LEFT,
		STRAFE_RIGHT
	};
}

namespace BODY {
	enum BODY {
		IDLE,
		MOVE,
		SHOOT,
		RELOAD
	};
}

struct {
	Animation b_idle{ 0, 19, 0.06f, 300, 200, false, 0 };
	Animation b_move{ 0, 19, 0.06f, 300, 200, false, 1 };
	Animation b_shoot{ 0,  2, 0.06f, 300, 200, false, 2 };

	Animation f_idle{ 0,  0,  0.0f, 203, 177, false, 0 };
	Animation f_run{ 0, 19, 0.03f, 203, 177, false, 1 };
	Animation f_sleft{ 0, 19, 0.02f, 203, 177, false, 2 };
	Animation f_sright{ 0, 19, 0.02f, 203, 177, false, 3 };

}animations;

std::unordered_map<int, Animation> animationsFeet{
	std::make_pair(FEET::IDLE, animations.f_idle),
	std::make_pair(FEET::RUN, animations.f_run),
	std::make_pair(FEET::STRAFE_LEFT, animations.f_sleft),
	std::make_pair(FEET::STRAFE_RIGHT, animations.f_sright)
};

std::unordered_map<int, Animation> animationsBody{
	std::make_pair(BODY::IDLE,animations.b_idle),
	std::make_pair(BODY::MOVE, animations.b_move),
	std::make_pair(BODY::SHOOT, animations.b_shoot)
};

Player::Player(Game* game, IPlayerInput* playerInput,bool r) : game(game), playerInput(playerInput), remote(r),
	hitboxRadius(PLAYER_RADIUS), healthbar(sf::Vector2f(50,10)),
	playerModel(SpriteDefinition(animationsFeet/*,"survivor_feet"*/,sf::Vector2f(100, 70),0.3,this->game->texmgr.getRef("survivor_feet")),
		SpriteDefinition(animationsBody/*,"survivor_rifle"*/, sf::Vector2f(100, 120), 0.3, this->game->texmgr.getRef("survivor_rifle")))
{
	hitpoints = 100;
	this->weapons.emplace_back(std::make_shared<Shotgun>(this->game->texmgr));
	this->weapons.emplace_back(std::make_shared<SMG>(this->game->texmgr));
	this->weapons.emplace_back(std::make_shared<Railgun>(this->game->texmgr));
	this->weapons.emplace_back(std::make_shared<FlameThrower>(this->game->texmgr));
	weapon = weapons.begin();
	changedWeapon.emit(weapon->get()->name);

	hitbox = sf::ConvexShape(4);
	hitbox.setPoint(0, sf::Vector2f(0, 0));
	hitbox.setPoint(1, sf::Vector2f(0, PLAYER_RADIUS));
	hitbox.setPoint(2, sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS));
	hitbox.setPoint(3, sf::Vector2f(PLAYER_RADIUS, 0));
	hitbox.setOutlineColor(sf::Color::White);
	hitbox.setOutlineThickness(2);
	hitbox.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);
	hitbox.setRotation(45);

	healthbar.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS - 10));

	if (this->remote == false)
	{
		Console::Instance() << "Created player with " << entityID << std::endl;
		if (this->game->networkmgr.type == "client")
		{
			this->game->networkmgr.sendInitAlive(entityID);
		}
	}
}

Player::Player(Game* game, IPlayerInput* playerInput,const Guid& guid) : Player(game,playerInput,true) 
{
	this->entityID = guid;
	healthbar.show();
	Console::Instance() << "Created remote Player with " << entityID << std::endl;
}


Player::~Player()
{
}

void Player::draw(sf::RenderWindow& window)
{
	weapon->get()->draw(window);
	
	window.draw(playerModel);
	//window.draw(hitbox);
	
	window.draw(healthbar);
}

void Player::setPosition(sf::Vector2f pos)
{
	hitbox.setPosition(pos);

	playerModel.setPosition(pos);

	weapon->get()->setPosition(pos);

	healthbar.setPosition(pos);
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

	sf::Vector2f directionOffset = newPos - this->getPosition();
	sf::Vector2f center = getPosition();

	this->hitbox.setRotation(angleOffset+45);
	this->playerModel.setRotation(angleOffset);
		
	this->hitbox.setPosition(newPos);
	this->playerModel.setPosition(newPos);

	this->healthbar.setPosition(newPos);
	
	this->attackingAngle = angleOffset;

	weapon->get()->attachedSetPosition(newPos, angleOffset);
	weapon->get()->update(dt);
	//weapon->get()->triggered = (this->playerInput->getAction() > 0) ? true : false;

		
	
	
	bool moving;
	if (directionOffset != sf::Vector2f(0, 0)) {

		float angle = V2Tools::crossProduct(sf::Vector2f(cos(-angleOffset* M_PI / 180.0f), -sin(-angleOffset* M_PI / 180.0f)), V2Tools::normalize(directionOffset));
		if (angle >= 0.6)
		{
			playerModel.spriteDefs[0].setnextState(FEET::STRAFE_RIGHT);
		}
		else if (angle <= -0.6)
		{
			playerModel.spriteDefs[0].setnextState(FEET::STRAFE_LEFT);
		}
		else
		{
			playerModel.spriteDefs[0].setnextState(FEET::RUN);
		}
		
		moving = true;
	}
	else {
		playerModel.spriteDefs[0].setnextState(FEET::IDLE);
		moving = false;
	}

	if (this->playerInput->getAction() > 0)
	{
		weapon->get()->enterState<Discharge>();
		playerModel.spriteDefs[1].setnextState(BODY::SHOOT);
		changedAmmo.emit(weapon->get()->ammo);
	}
	else
	{
		if (moving)
			playerModel.spriteDefs[1].setnextState(BODY::MOVE);
		else
			playerModel.spriteDefs[1].setnextState(BODY::IDLE);
	}
	
	std::vector<int> events = this->playerInput->getEvent();
	for (auto& ev : events)
	{
		switch (ev)
		{
		case 0:
		{
			this->nextWeapon();
			changedWeapon.emit(weapon->get()->name);
			changedAmmo.emit(weapon->get()->ammo);
			break;
		}
		case 1:
		{
			this->prevWeapon();
			changedWeapon.emit(weapon->get()->name);
			changedAmmo.emit(weapon->get()->ammo);
			break;
		}
		case 2:
		{
			this->weapon = weapons.begin() + 2;
			changedWeapon.emit(weapon->get()->name);
			changedAmmo.emit(weapon->get()->ammo);
			break;
		}
		case 3:
		{
			this->weapon = weapons.begin() + 1;
			changedWeapon.emit(weapon->get()->name);
			changedAmmo.emit(weapon->get()->ammo);
			break;
		}
		case 4:
		{
			this->weapon = weapons.begin();
			changedWeapon.emit(weapon->get()->name);
			changedAmmo.emit(weapon->get()->ammo);
			break;
		}
		case 5:
		{
			this->weapon = weapons.begin() + 3;
			changedWeapon.emit(weapon->get()->name);
			changedAmmo.emit(weapon->get()->ammo);
			break;
		}
		default:
			break;
		}
	}

	if (this->playerInput->getAlternateAction() > 0 && this->playerInput->getAction() == 0)
	{
		healingTimer += dt;
		if (healingTimer >= 1.0f && hitpoints < 100)
		{
			hitpoints += 5;
			changedHitpoints.emit(hitpoints);
			healingTimer = 0;
			std::cout << hitpoints << std::endl;
		}
	}
	else if (this->playerInput->getAlternateAction() == 0)
	{
		healingTimer = 0;
	}
	this->healthbar.setValue(this->hitpoints / 100.0f);

	playerModel.update(dt);

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
			changedHitpoints.emit(hitpoints);
			other.isDeletable = true;
		}
		if (type == 0)
		{
			float moveBy = PLAYER_SPEED * dt;

			float angle = atan2(displace.y, displace.x) * 180 / M_PI;
			float length = V2Tools::length(displace);

			this->hitbox.move(displace * moveBy);
			this->playerModel.move(displace * moveBy);
			(*weapon)->attachedMove(displace * moveBy, playerInput->getAngle());

			this->healthbar.move(displace * moveBy);
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
size_t Player::getPointCount() const
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

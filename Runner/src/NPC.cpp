#include "stdafx.h"
#include "NPC.h"
#include "SAT.h"
#include "MapTools.h"
#include "V2Tools.h"
#include "math.h"
#include <string>
#include <memory>


NPC::NPC(Game* game, sf::Vector2f pos) : game(game),
npcModel(PLAYER_RADIUS), npcHead(PLAYER_RADIUS / 2), isAttacking(false), hitboxRadius(PLAYER_RADIUS), healthbar(sf::Vector2f(50,10)), weapon(this->game->texmgr)
{
	hitpoints = 100;

	hitbox = sf::ConvexShape(4);
	hitbox.setPoint(0, sf::Vector2f(0, 0));
	hitbox.setPoint(1, sf::Vector2f(0, PLAYER_RADIUS));
	hitbox.setPoint(2, sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS));
	hitbox.setPoint(3, sf::Vector2f(PLAYER_RADIUS, 0));
	//hitbox = new sf::RectangleShape(sf::Vector2f(PLAYER_RADIUS * 2, PLAYER_RADIUS * 2));
	hitbox.setOutlineColor(sf::Color::White);
	hitbox.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);
	hitbox.setRotation(45);

	npcModel.setFillColor(sf::Color::Green);
	npcModel.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
	npcModel.setScale(0.5, 1);

	npcHead.setFillColor(sf::Color::Black);
	npcHead.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	healthbar.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS - 10));
	healthbar.show();

	setPosition(pos);

	this->game->networkmgr.AddGameEntity(entityID);
	this->game->networkmgr.broadcastGameObjects();
}


NPC::~NPC()
{
}

void NPC::draw(sf::RenderWindow& window)
{
	if (isAttacking)
		weapon.draw(window);
	window.draw(npcModel);
	window.draw(npcHead);

	window.draw(healthbar);
}

void NPC::setPosition(sf::Vector2f pos)
{
	hitbox.setPosition(pos);

	npcModel.setPosition(pos);
	npcHead.setPosition(pos);

	weapon.setPosition(pos);

	healthbar.setPosition(pos);
}

sf::Vector2f NPC::getPosition() const
{
	return npcModel.getPosition();
}

void NPC::moveToTarget(std::deque<sf::Vector2f> target)
{
	this->targetList = target;
	this->targetList.pop_front();
}

void NPC::moveToTarget(sf::Vector2f target)
{
	this->targetList.emplace_front(target);
}

void NPC::shootAtTarget(sf::Vector2f target)
{
	this->target = target;
	this->isAttacking = true;
}

void NPC::update(float dt)
{
	if (hitpoints <= 0) this->isDeletable = true;
	
	float moveBy = PLAYER_SPEED * dt;	
	
	this->hitbox.setRotation(viewAngle + 45);
	this->npcModel.setRotation(viewAngle);
	this->npcHead.setRotation(viewAngle);

	sf::Vector2f directionOffset = direction * moveBy;
	this->hitbox.move(directionOffset);
	this->npcModel.move(directionOffset);
	this->npcHead.move(directionOffset);

	this->healthbar.move(directionOffset);

	weapon.attachedMove(directionOffset, viewAngle);
	weapon.update(dt);

	this->updateWithDeltaTime(dt);	

	if (this->game->networkmgr.networkGameObjects.size() != 0)
	{
		sf::Vector2f newPos = this->getPosition();
		NetworkPlayerEvent event;
		event.x = newPos.x;
		event.y = newPos.y;
		event.angle = this->viewAngle;
		event.hitpoints = hitpoints;
		event.action = 0;
		event.alternateAction = 0;
		event.equippedWeapon = 1;
		event.entityID = this->entityID;

		this->game->networkmgr.queueEvent(event);
	}
}

void NPC::collide(IGameEntity& other, unsigned int type, float dt)
{
	sf::Vector2f displace = Collision::SATCollision(this, &other);
	if (displace != sf::Vector2f(0, 0))
	{
		if (type == 1)
		{
			this->hitpoints -= other.damage;
			other.isDeletable = true;
			this->healthbar.setValue(this->hitpoints / 100.0f);
		}
		if (type == 0)
		{
			float moveBy = PLAYER_SPEED * dt;

			float angle = atan2(displace.y, displace.x) * 180 / M_PI;
			float length = V2Tools::length(displace);

			this->hitbox.move(displace * moveBy);
			this->npcModel.move(displace * moveBy);
			this->npcHead.move(displace * moveBy);
			this->weapon.attachedMove(displace * moveBy, this->attackingAngle);

			this->healthbar.move(displace * moveBy);
		}
	}
}

sf::Vector2f NPC::getPoint(int i) const
{
	return hitbox.getPoint(i);
}
unsigned int NPC::getPointCount() const
{
	return hitbox.getPointCount();
}
sf::Transform NPC::getTransform() const
{
	return hitbox.getTransform();
}

float NPC::getMinDistance() const
{
	return PLAYER_RADIUS;
}


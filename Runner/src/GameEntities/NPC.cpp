#include "stdafx.h"
#include "NPC.h"
#include "Algorithms/SAT.h"
#include "Map/MapTools.h"
#include "Algorithms/V2Tools.h"
#include "Network/NetworkManager.h"
#include <cmath>
#include <string>
#include <memory>


NPC::NPC(Game* game, sf::Vector2f pos) : game(game), IAtomicEntity(4),
npcModel(PLAYER_RADIUS), npcHead(PLAYER_RADIUS / 2), isAttacking(false), hitboxRadius(PLAYER_RADIUS), healthbar(sf::Vector2f(50,10)), weapon(*this->game->texmgr)
{
	hitpoints = 100;

	this->setPoint(0, sf::Vector2f(0, 0));
	this->setPoint(1, sf::Vector2f(0, PLAYER_RADIUS));
	this->setPoint(2, sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS));
	this->setPoint(3, sf::Vector2f(PLAYER_RADIUS, 0));
	this->setOutlineColor(sf::Color::White);
	this->setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	npcModel.setFillColor(sf::Color::Green);
	npcModel.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
	npcModel.setScale(0.5, 1);
	npcModel.setPosition(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	npcHead.setFillColor(sf::Color::Black);
	npcHead.setOrigin(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);
	npcHead.setPosition(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	healthbar.setOrigin(sf::Vector2f(25, -PLAYER_RADIUS - 10));
	healthbar.show();

	facingDot.setRadius(5);
	facingDot.setOrigin(5, 5);
	facingDot.setFillColor(sf::Color::Red);
	facingDot.setPosition(PLAYER_RADIUS / 2, PLAYER_RADIUS / 2);

	setPosition(pos);

	//this->game->networkmgr.AddGameEntity(entityID);
	//this->game->networkmgr.broadcastGameObjects();
}


NPC::~NPC()
{
}

void NPC::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	if (isAttacking)
		target.draw(weapon, states);
	target.draw(npcModel, states);
	target.draw(npcHead, states);
	target.draw(healthbar);
	target.draw(facingDot, states);
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
	
	this->setRotation(viewAngle);
	this->healthbar.setPosition(this->getPosition());
	sf::Vector2f directionOffset = direction * moveBy;
	this->move(directionOffset);
	

	//weapon.attachedMove(directionOffset, viewAngle);
	weapon.attachedSetPosition(this->getPosition(), viewAngle);
	weapon.update(dt);

	this->updateWithDeltaTime(dt);

	//if (this->game->networkmgr.networkGameObjects.size() != 0)
	if( this->game->networkmgr->peers.size() != 0)
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

		this->game->networkmgr->queueEvent(event);
	}
}

void NPC::collide(IAtomicEntity& other, unsigned int type, float dt)
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

			this->move(displace * moveBy);
			//this->npcModel.move(displace * moveBy);
			//this->npcHead.move(displace * moveBy);
			this->weapon.attachedMove(displace * moveBy, this->attackingAngle);

			//this->healthbar.move(displace * moveBy);
		}
	}
}

float NPC::getMinDistance() const
{
	return PLAYER_RADIUS;
}


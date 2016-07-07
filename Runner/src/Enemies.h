#pragma once
#include "NPC.h"
#include  "Player.h"
#include "GameObjectManager.h"

#include <map>
#include <memory>

class IdleState;
class FightingState;

class Enemies :
	public NPC
{
public:
	Enemies(Game* game, sf::Vector2f pos) : NPC(game, pos)
	{
		this->addState<FightingState>(this);
		this->addState<IdleState>(this);
		this->enterState<IdleState>();
		//this->addState<EvadingState>(this);
		//this->addState<PursuingState>(this);

		
	};
	~Enemies();

	//std::map<int, std::shared_ptr<IAtomicEntity>> visibleProjectiles;
	std::map<int, std::shared_ptr<Player>> visibleEnemies;
	std::shared_ptr<Player> target;
	std::stack<std::pair<sf::Vector2f, float>> targetHistory;
	
	sf::Vector2f evasionDirection;
	sf::Vector2f lastKnownEnemyLocation;
	sf::Vector2f targetLocation;	

	void updateVisibleEnemies(const GameObjectManager<Player>& players)
	{
		//search all visible area und update enemyMap;
		for (auto& p : players.objects)
		{
			visibleEnemies.insert(std::make_pair((int)V2Tools::distance(p.second->getPosition(), this->getPosition()),p.second));
		}
	}

	void updateVisibleProjectiles(const GameObjectManager<Projectile>& projectiles)
	{
		//search all projectiles for dangerous projectiles
		evasionDirection = sf::Vector2f(0, 0);
		for (auto& p : projectiles.objects)
		{
			sf::Vector2f connection = V2Tools::normalize(this->getPosition() - p.second->getPosition());
			sf::Vector2f direction = p.second->getDirection();
			if (V2Tools::dotProduct(direction, connection) > 0.0f)
			{
				float angle = V2Tools::crossProduct(direction, connection);
				if (angle <= 0.3 && angle >= -0.3)
					evasionDirection += connection;
			}
		}
		evasionDirection = V2Tools::normalize(evasionDirection);
	}
};

class EnemyState : public BaseState
{
public:
	Enemies* thisEntity;
	EnemyState(Enemies* t) : thisEntity(t) {}
};

class IdleState : public EnemyState
{
public:
	using EnemyState::EnemyState;
	virtual void updateState(float dt)
	{
		//all move orders
		if (!thisEntity->visibleEnemies.empty())
			this->_stateMachine->enterState<FightingState>();
	}
	virtual bool isValidNextState(BaseState* state)
	{
		if (typeid(*state) == typeid(FightingState)
			//|| typeid(*state) == typeid(EvadingState)
			//|| typeid(*state) == typeid(PursuingState)
			)
		{
			return true;
		}
		else {
			return false;
		}
	}

	virtual void enterState(BaseState* state) {}; 

	virtual void exitState(BaseState* state) {};
};

class FightingState : public EnemyState
{
	
public:
	using EnemyState::EnemyState;

	void predictTargetLocation(float dt)
	{
		//predict targetlocation, for know, no prediction
		thisEntity->targetLocation = thisEntity->target->getPosition();
	}

	//void checkVisibilityOfTarget()
	//{
	//	//if not visible, stop fighting
	//}

	void setMoveAndOrientation()
	{
		thisEntity->direction = V2Tools::normalize(thisEntity->targetLocation - thisEntity->getPosition());
		thisEntity->viewAngle = atan2(thisEntity->direction.y, thisEntity->direction.x) * 180 / M_PI;

		float dis = V2Tools::distance(thisEntity->targetLocation, thisEntity->getPosition());
		if (dis <= PLAYER_RADIUS* 10.0f)
		{
			thisEntity->direction = sf::Vector2f(0, 0);
		}

		thisEntity->direction += thisEntity->evasionDirection;
		thisEntity->direction = V2Tools::normalize(thisEntity->direction);
	}

	virtual void enterState(BaseState* previousState)
	{
		//choose target from list
		thisEntity->target = thisEntity->visibleEnemies.begin()->second;
	}
	virtual void exitState(BaseState* nextState)
	{

	}
	virtual void updateState(float dt)
	{
		//FightingState::updateState(dt);
		//stay in Range of Target;
		predictTargetLocation(dt);
		setMoveAndOrientation();
		thisEntity->weapon.enterState<Discharge>();
		
	}
	virtual bool isValidNextState(BaseState* state)
	{
		if (typeid(*state) == typeid(IdleState)
			//|| typeid(*state) == typeid(EvadingState)
			//|| typeid(*state) == typeid(PursuingState)
			)
		{
			return true;
		}
		else {
			return false;
		}
	}
};




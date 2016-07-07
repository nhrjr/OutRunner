#pragma once
#include "GameEntities/NPC.h"
#include "GameEntities/Player.h"
#include "Core/GameObjectManager.h"
#include <map>
#include <memory>

class IdleState;
class FightingState;
//class Enemies;

class Enemies :
	public NPC
{
public:
	Enemies(Game* game, sf::Vector2f pos) : NPC(game, pos)
	{
		this->addState<IdleState>(this);
		this->addState<FightingState>(this);
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
	virtual void updateState(float dt);
	virtual bool isValidNextState(BaseState* state);
	virtual void enterState(BaseState* previousState) {};
	virtual void exitState(BaseState* nextState) {};
};

class FightingState : public EnemyState
{

public:
	using EnemyState::EnemyState;

	void predictTargetLocation(float dt);
	void setMoveAndOrientation();
	virtual void enterState(BaseState* previousState);
	virtual void exitState(BaseState* nextState);
	virtual void updateState(float dt);
	virtual bool isValidNextState(BaseState* state);
};




#include "stdafx.h"
#include "Enemies.h"


Enemies::~Enemies()
{
}

void IdleState::updateState(float dt)
{
	//all move orders
	if (!thisEntity->visibleEnemies.empty())
		this->_stateMachine->enterState<FightingState>();
}
   
bool IdleState::isValidNextState(BaseState* state)
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


	void FightingState::predictTargetLocation(float dt)
	{
		//predict targetlocation, for know, no prediction
		thisEntity->targetLocation = thisEntity->target->getPosition();
	}

	void FightingState::setMoveAndOrientation()
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

	 void FightingState::enterState(BaseState* previousState)
	{
		//choose target from list
		thisEntity->target = thisEntity->visibleEnemies.begin()->second;
	}
	 void FightingState::exitState(BaseState* nextState)
	{

	}
	 void FightingState::updateState(float dt)
	{
		//FightingState::updateState(dt);
		//stay in Range of Target;
		predictTargetLocation(dt);
		setMoveAndOrientation();
		thisEntity->weapon.enterState<Discharge>();
		
	}
	 bool FightingState::isValidNextState(BaseState* state)
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

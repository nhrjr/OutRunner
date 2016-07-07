#pragma once
#include "IGameState.h"

#include "Core/GameLogicManager.h"
#include "Core/CollisionManager.h"
#include "Core/Console.h"

#include "Algorithms/SAT.h"
#include "Algorithms/signal_slot.h"
#include "Algorithms/StateMachine.h"

#include "GameEntities/Player.h"
#include "GameEntities/NPC.h"
#include "GameEntities/Enemies.h"

#include "Input/IPlayerInput.h"

#include "Map/Map.h"
#include "Map/MapHitbox.h"

#include "Gui/GUI.h"
#include "Gui/Cursor.h"

#include <vector>
#include <string>




class GameStatePlaying :
	public IGameState
{
public:
	GameStatePlaying(Game* game, IPlayerInput* player);
	~GameStatePlaying();

	virtual void update(float dt);
	virtual void draw(float dt);
	virtual void handleInput();
	virtual bool end();

	void setGameGUI();
	void resize(sf::Event& event);

	bool moveNPC = false;

	std::unique_ptr<StateMachine> stateMachine;

private:
	bool isDeletable;
	bool hasFocus = true;

	ActionState actionState;
	std::shared_ptr<Player> player;
	Cursor cursor;
	IPlayerInput* playerInput;

	//colDrawHelper helper;

	sf::View gameView;
	sf::View guiView;

	sf::Vector2i panningAnchor;
	float zoomLevel;

	//

	GuiPlaying gui;

	GameLogicManager gameLogicManager;

	Map map;

	float aliveTimer = 0;

	
	
};

class DownState : public BaseState
{
public:
	virtual void enterState(BaseState* previousState)
	{
		if (previousState != nullptr)
			Console::Instance() << "Downstate enter() Entered from " << typeid(*previousState).name() << std::endl;
	};
	virtual bool isValidNextState(BaseState* state)
	{
		if (state != nullptr)
			Console::Instance() << "Is this state a next valid state? : " << typeid(*state).name() << std::endl;
		return true;
	};

	virtual void exitState(BaseState* nextState)
	{
		if (nextState != nullptr)
			Console::Instance() << "Exit LowState to next state " << typeid(*nextState).name() << std::endl;
	};
	std::string downStateString = "!!!";
};

class LowState : public DownState
{
public:
	virtual bool isValidNextState(BaseState* state)
	{
		if (state != nullptr)
		Console::Instance() << "Is this state a next valid state? : " << typeid(*state).name() << std::endl;
		return true;
	};

	virtual void exitState(BaseState* nextState)
	{
		if (nextState != nullptr)
		Console::Instance() << "Exit LowState to next state " << typeid(*nextState).name() << std::endl;
		Console::Instance() << downStateString << std::endl;
	};
};

class HighState : public DownState
{
public:
	virtual bool isValidNextState(BaseState* state)
	{
		if(state != nullptr)
		Console::Instance() << "Is this state a next valid state? : " << typeid(*state).name() << std::endl;
		return true;
	};

	virtual void exitState(BaseState* nextState)
	{
		if (nextState != nullptr)
		Console::Instance() << "Exit HighState to next state " << typeid(*nextState).name() << std::endl;
		Console::Instance() << downStateString << std::endl;
	};
};
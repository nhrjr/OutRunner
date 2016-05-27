#pragma once
#include "IGameState.h"
#include "Map.h"
#include "Player.h"
#include "NPC.h"
#include "Enemies.h"
#include "Cursor.h"
#include "IPlayerInput.h"
#include "GameLogicManager.h"
#include "CollisionManager.h"
#include "SAT.h"
#include "MapHitbox.h"
#include "Console.h"

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

	void setGuiSystem();
	void resize(sf::Event& event);

	bool moveNPC = false;

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

	std::map<std::string, GUI> guiSystem;

	GameLogicManager gameLogicManager;

	Map map;

	float aliveTimer = 0;
};


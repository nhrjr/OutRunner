#pragma once
#include "IGameState.h"
#include "Map.h"
#include "Player.h"
#include "NPC.h"
#include "Enemies.h"
#include "Cursor.h"
#include "IPlayerInput.h"
#include "SAT.h"
#include "MapHitbox.h"

#include <vector>




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

	//void testPathfinder();
	void drawTestPath();
	std::deque<sf::Vector2f> testPath;
	bool moveNPC = false;


private:
	bool shouldEnd;

	ActionState actionState;
	Player player;
	//NPC npc;
	Cursor cursor;
	IPlayerInput* playerInput;

	colDrawHelper helper;

	sf::View gameView;
	sf::View guiView;

	sf::Vector2i panningAnchor;
	float zoomLevel;

	std::map<std::string, GUI> guiSystem;

	std::vector<Bullet> bullets;
	std::vector<Enemies> enemies;

	Map map;
};


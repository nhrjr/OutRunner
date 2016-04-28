#pragma once
#include "stdafx.h"
#include "IGameState.h"

#include "MainMenu.h"
#include "GameObjectManager.h"
#include "GameSettingConstants.h"
#include "Tile.h"
#include "TextureManager.h"
#include "GUI.h"

#include <string>
#include <stack>
#include <queue>

class IGameState;

struct Switches {
	bool showPolygon = false;
	bool showPathfinder = false;
	bool showPath = true;
	bool showHitboxHighlights = false;
};

class Game
{
public:
	Game();
	~Game();

	void start();

	void pushState(IGameState* state);
	void popState();
	void changeState(IGameState* state);
	IGameState* peekState();

	sf::RenderWindow window;
	sf::Sprite background;

	//GameObjectManager objmgr;
	TextureManager texmgr;
	Switches switches;

	
	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> styleSheets;
	std::map<std::string, sf::Font> fonts;
	std::queue<IGameState*> states;

private:
	void gameLoop(sf::Clock&);

	void loadTextures();
	void loadTiles();
	void loadFonts();
	void loadStyleSheets();
	
		
};


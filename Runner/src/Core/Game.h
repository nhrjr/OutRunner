#pragma once
// #include "GameStates/IGameState.h"
//#include "GameObjectManager.h"
#include "GameSettingConstants.h"
#include "Map/Tile.h"
// #include "TextureManager.h"
#include "Gui/GuiElements.h"
// #include "Network/NetworkManager.h"

#include <string>
#include <stack>
#include <queue>
#include <memory>

class IGameState;
class TextureManager;
class NetworkManager;

struct Switches {
	bool showPolygon = false;
	bool showPathfinder = false;
	bool showPath = false;
	bool showHitboxHighlights = false;
	bool playerInvincible = false;
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
	// TextureManager texmgr;
	Switches switches;
	// NetworkManager networkmgr;

	std::unique_ptr<NetworkManager> networkmgr;
	std::unique_ptr<TextureManager> texmgr;
	
	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, ButtonStyle> styleSheets;
	std::map<std::string, sf::Font> fonts;
	std::queue<IGameState*> states;

private:
	void gameLoop(sf::Clock&);

	void loadTextures();
	void loadTiles();
	void loadFonts();
	void loadStyleSheets();
	
		
};


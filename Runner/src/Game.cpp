#include "stdafx.h"
#include "Game.h"
#include "Console.h"

#include "GameStateStart.h"
#include "GameStateEditor.h"

#include "CycleCounter.h"

//#include <Windows.h>

Game::Game()
{
	this->loadTextures();
	this->loadTiles();
	this->loadFonts();
	this->loadStyleSheets();
	
	this->window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), GAME_NAME);

	this->background.setTexture(this->texmgr.getRef("background"));

	Command c = [this](std::vector<std::string> str) -> std::string {
		this->switches.showPath = (this->switches.showPath == true) ? false : true;
		return std::string("Set showPath to " + std::to_string(this->switches.showPath));
	};
	Console::Instance().Add("togglePath", c);

	c = [this](std::vector<std::string> str) -> std::string {
		this->switches.showPathfinder = (this->switches.showPathfinder == true) ? false : true;
		return std::string("Set showPathfinder to " + std::to_string(this->switches.showPathfinder));
	};
	Console::Instance().Add("togglePathfinder", c);

	c = [this](std::vector<std::string> str) -> std::string {
		this->switches.showHitboxHighlights = (this->switches.showHitboxHighlights == true) ? false : true;
		return std::string("Set showHitboxHighlights to " + std::to_string(this->switches.showHitboxHighlights));
	};
	Console::Instance().Add("toggleHitBoxes", c);

	c = [this](std::vector<std::string> str) -> std::string {
		this->switches.showPolygon = (this->switches.showPolygon == true) ? false : true;
		return std::string("Set showPolygon to " + std::to_string(this->switches.showPolygon));
	};
	Console::Instance().Add("togglePolygon", c);

	c = [this](std::vector<std::string> str) -> std::string {
		this->switches.playerInvincible = (this->switches.playerInvincible == true) ? false : true;
		return std::string("Set playerInvincible to " + std::to_string(this->switches.playerInvincible));
	};
	Console::Instance().Add("p_toggleInvincible", c);

	c = [this](std::vector<std::string> str) -> std::string {
		this->window.close();
		return std::string("Exit");
	};
	Console::Instance().Add("exit", c);

}

Game::~Game()
{
	while (!this->states.empty()) popState();
}

void Game::pushState(IGameState* state) {
	this->states.push(state);
}

void Game::popState() {
	delete this->states.front();
	this->states.pop();
}

void Game::changeState(IGameState* state) {
	if (!this->states.empty())
		popState();
	pushState(state);
}

IGameState* Game::peekState() {
	if (this->states.empty())
		return nullptr;
	//return this->states.top();
	return this->states.front();
}


void Game::start() {


	pushState(new GameStateStart(this));

	sf::Clock clock;

	this->gameLoop(clock);

	window.close();
}


void Game::gameLoop(sf::Clock& clock) {
	
		while (this->window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		if (this->peekState() == nullptr) continue;

		networkmgr.processSocket(dt);
		peekState()->handleInput();
		peekState()->update(dt);
		networkmgr.sendEvents();

		if (peekState()->end())
		{
			popState();
			if (this->states.empty())
			{
				networkmgr.reset();
				pushState(new GameStateStart(this));
			}

		}
		peekState()->draw(dt);		
		this->window.display();


#ifdef DEBUG_CYCLECOUNT
		DebugCycleCounterResetAndPrint(dt);
#endif
		
		//std::cout << "Megacycles / Frame: " << cyclesElapsed / ( 1000 * 1000 ) << std::endl;
	}
}

void Game::loadTextures() {
	texmgr.loadTexture("Concrete", "textures/concrete.png");
	texmgr.loadTexture("Wall", "textures/wall.png");
	texmgr.loadTexture("Water", "textures/water.png");
	texmgr.loadTexture("background", "textures/background.png");
	texmgr.loadTexture("cursor", "textures/cursor.png");
	texmgr.loadTexture("survivor_rifle", "textures/survivor_rifle.png");
	texmgr.loadTexture("survivor_feet", "textures/survivor_feet.png");
	texmgr.loadTexture("SMG", "textures/rifle_muzzleflash.png");
	texmgr.loadTexture("Shotgun", "textures/shotgun_muzzleflash.png");
	texmgr.loadTexture("Railgun", "textures/rifle_muzzleflash.png");
	texmgr.loadTexture("FlameThrower", "textures/rifle_muzzleflash.png");
}

void Game::loadTiles() {
	Animation concreteAnim(0, 0, 0.0f, GAME_TILESIZE, GAME_TILESIZE, true, 3);
	Animation wallAnim(0, 0, 0.0f, GAME_TILESIZE, GAME_TILESIZE, true, 2);
	Animation waterAnim(0,7,1.0f, GAME_TILESIZE, GAME_TILESIZE, true, 1);

	tileAtlas.emplace("Void", Tile(GAME_TILESIZE, GAME_TILESIZE, TileType::VOID));
	tileAtlas.emplace("Concrete",Tile(GAME_TILESIZE, GAME_TILESIZE, texmgr.getRef("Concrete"), concreteAnim , TileType::CONCRETE,3));
	tileAtlas.emplace("Wall", Tile(GAME_TILESIZE, GAME_TILESIZE, texmgr.getRef("Wall"), wallAnim , TileType::WALL,2));
	tileAtlas.emplace("Water", Tile(GAME_TILESIZE, GAME_TILESIZE, texmgr.getRef("Water"), waterAnim, TileType::WATER,1));
}

void Game::loadFonts()
{
	sf::Font font;
	font.loadFromFile("media/font.ttf");
	this->fonts["main_font"] = font;
}

void Game::loadStyleSheets()
{
	this->styleSheets["button_text"] = ButtonStyle(&this->fonts.at("main_font"), 1.0f,
		sf::Color(0xc6, 0xc6, 0xc6), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00),
		sf::Color(0x61, 0x61, 0x61), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00));
	this->styleSheets["text"] = ButtonStyle(&this->fonts.at("main_font"), 0.0f,
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0xff, 0xff),
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0x00, 0x00));
}



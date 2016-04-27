#include "stdafx.h"
#include "Game.h"

#include "GameStateStart.h"
#include "GameStateEditor.h"

Game::Game()
{
	this->loadTextures();
	this->loadTiles();
	this->loadFonts();
	this->loadStyleSheets();
	

	this->window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), GAME_NAME);

	this->background.setTexture(this->texmgr.getRef("background"));

}


Game::~Game()
{
	while (!this->states.empty()) popState();
}

void Game::pushState(IGameState* state) {
	this->states.push(state);
}

void Game::popState() {
	//delete this->states.top();
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

		peekState()->handleInput();
		peekState()->update(dt);
		peekState()->draw(dt);		
		this->window.display();
		if (peekState()->end())
		{
			popState();
			if (this->states.empty())
				pushState(new GameStateStart(this));
		}
			
		//std::cout << "Framerate: " << 1 / dt << std::endl;
	}
}

void Game::loadTextures() {
	texmgr.loadTexture("Concrete", "textures/concrete.png");
	texmgr.loadTexture("Wall", "textures/wall.png");
	texmgr.loadTexture("Water", "textures/water.png");
	texmgr.loadTexture("background", "textures/background.png");
	texmgr.loadTexture("cursor", "textures/cursor.png");
}

void Game::loadTiles() {
	Animation staticAnim(0, 0, 0.0f);
	Animation waterAnim(0,7,1.0f);

	tileAtlas.emplace("Void", Tile(GAME_TILESIZE, GAME_TILESIZE, TileType::VOID));
	tileAtlas.emplace("Concrete",Tile(GAME_TILESIZE, GAME_TILESIZE, texmgr.getRef("Concrete"), { staticAnim }, TileType::CONCRETE,3));
	tileAtlas.emplace("Wall", Tile(GAME_TILESIZE, GAME_TILESIZE, texmgr.getRef("Wall"), { staticAnim }, TileType::WALL,2));
	tileAtlas.emplace("Water", Tile(GAME_TILESIZE, GAME_TILESIZE, texmgr.getRef("Water"), { waterAnim }, TileType::WATER,1));
}

void Game::loadFonts()
{
	sf::Font font;
	font.loadFromFile("media/font.ttf");
	this->fonts["main_font"] = font;
}

void Game::loadStyleSheets()
{
	this->styleSheets["button"] = GuiStyle(&this->fonts.at("main_font"), 1.0f,
		sf::Color(0xc6, 0xc6, 0xc6), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00),
		sf::Color(0x61, 0x61, 0x61), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00));
	this->styleSheets["text"] = GuiStyle(&this->fonts.at("main_font"), 0.0f,
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0xff, 0xff),
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0x00, 0x00));
}



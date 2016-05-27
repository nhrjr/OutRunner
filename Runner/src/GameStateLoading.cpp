#include "stdafx.h"
#include "GameStateLoading.h"
#include "GameStatePlaying.h"


GameStateLoading::GameStateLoading(Game* g)
{
	this->game = g;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, this->game->window.getSize().x, this->game->window.getSize().y);
	this->view.reset(viewRect);
	pos *= 0.5f;
	this->view.setCenter(pos);



	this->setGuiSystem();
}


GameStateLoading::~GameStateLoading()
{
}

void GameStateLoading::resize(sf::Event& event)
{
	sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
	sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);

	this->view.reset(viewRect);
	this->view.setCenter(pos*0.5f);

	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
		float(event.size.height) / float(this->game->background.getTexture()->getSize().y));

	//this->guiSystem.at("menu").setPosition(pos * 0.5f);
	//this->guiSystem.at("menu").show();
}

void GameStateLoading::setGuiSystem()
{
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(this->game->window.getSize().x) / float(this->game->background.getTexture()->getSize().x),
		float(this->game->window.getSize().y) / float(this->game->background.getTexture()->getSize().y));

	//this->guiSystem.emplace("menu", GUI(sf::Vector2f(GAME_MENU_BUTTON_WIDTH, GAME_MENU_BUTTON_HEIGHT), 4, false, game->styleSheets.at("button"), { std::make_pair("Play (Solo)","play_game_single"),std::make_pair("Play (Server)","play_game_server"), std::make_pair("Play (Client)","play_game_client"), std::make_pair("Load Editor", "load_game"), std::make_pair("Exit Game", "exit_game") }));
	//this->guiSystem.at("menu").setOrigin(GAME_MENU_BUTTON_WIDTH / 2, GAME_MENU_BUTTON_HEIGHT / 2);
	//this->guiSystem.at("menu").setPosition(sf::Vector2f(this->game->window.getSize().x, this->game->window.getSize().y) * 0.5f);
	//this->guiSystem.at("menu").show();
}

void GameStateLoading::draw(float dt) {
	this->game->window.setView(this->view);

	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);

	//for (auto gui : guiSystem) this->game->window.draw(gui.second);
}

void GameStateLoading::update(float dt) {
	//sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);
	//this->guiSystem.at("menu").highlight(this->guiSystem.at("menu").getEntry(mousePos));

	temp_FindServerTimer += dt;
	if (this->game->networkmgr.type == "client")
	{
		if (this->game->networkmgr.temp_FindServerAndLoadMapState == 0)
		{
			if (temp_FindServerTimer >= 1)
			{
				this->game->networkmgr.findServer();
				temp_FindServerTimer = 0;
			}
			
		}
		if (this->game->networkmgr.temp_FindServerAndLoadMapState == 2)
		{
			//this->map.loadFromDiskPlaying(GAME_MAP_NAME_REMOTE, this->game->tileAtlas);
			this->game->pushState(new GameStatePlaying(this->game, new MKInput(this->game)));
			this->isDeletable = true;
			this->game->networkmgr.temp_FindServerAndLoadMapState = 3;
		}
		
	}
}

void GameStateLoading::handleInput()
{
}

bool GameStateLoading::end()
{
	return this->isDeletable;
}

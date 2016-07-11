#include "stdafx.h"
#include "GameStateStart.h"
#include "GameStateEditor.h"
#include "GameStatePlaying.h"
#include "GameStateLoading.h"

#include <utility>



GameStateStart::GameStateStart(Game* game) : isDeletable(false)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, static_cast<float>(this->game->window.getSize().x), static_cast<float>(this->game->window.getSize().y));
	this->view.reset(viewRect);
	pos *= 0.5f;
	this->view.setCenter(pos);

	this->setGameGUI();

}

void GameStateStart::setGameGUI()
{
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		static_cast<float>(this->game->window.getSize().x) / static_cast<float>(this->game->background.getTexture()->getSize().x),
		static_cast<float>(this->game->window.getSize().y) / static_cast<float>(this->game->background.getTexture()->getSize().y));

	sf::Vector2f dim(GAME_MENU_BUTTON_WIDTH, GAME_MENU_BUTTON_HEIGHT);
	int padding = 4;
	std::vector<std::shared_ptr<GuiBasicElement>> menu = {
		std::make_shared<GuiButtonText>("Singleplayer",dim,padding, game->styleSheets.at("button_text"), "play_game_single"),
		std::make_shared<GuiButtonText>("Host Multiplayer",dim,padding,game->styleSheets.at("button_text"), "play_game_server"),
		std::make_shared<GuiButtonText>("Join Multiplayer",dim,padding,game->styleSheets.at("button_text"), "play_game_client"),
		std::make_shared<GuiButtonText>("Load Editor", dim,padding,game->styleSheets.at("button_text"),"load_game"),
		std::make_shared<GuiButtonText>("Exit Game",dim,padding,game->styleSheets.at("button_text"), "exit_game")
	};
	this->guiElements.emplace("menu", std::make_shared<GuiList>(menu, false));
	this->guiElements.at("menu")->setOrigin(GAME_MENU_BUTTON_WIDTH / 2, GAME_MENU_BUTTON_HEIGHT / 2);
	this->guiElements.at("menu")->setPosition(sf::Vector2f(static_cast<float>(this->game->window.getSize().x * 0.5f), static_cast<float>(this->game->window.getSize().y) * 0.5f));
	this->guiElements.at("menu")->show();
}

void GameStateStart::resize(sf::Event& event)
{
	sf::Vector2f pos = sf::Vector2f(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
	sf::FloatRect viewRect(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));

	this->view.reset(viewRect);
	this->view.setCenter(pos*0.5f);

	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		static_cast<float>(event.size.width) / static_cast<float>(this->game->background.getTexture()->getSize().x),
		static_cast<float>(event.size.height) / static_cast<float>(this->game->background.getTexture()->getSize().y));

	this->guiElements.at("menu")->setPosition(pos * 0.5f);
	this->guiElements.at("menu")->show();
}


GameStateStart::~GameStateStart()
{
}

void GameStateStart::draw(float dt) {
	this->game->window.setView(this->view);

	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);

	for (auto gui : guiElements) this->game->window.draw(*gui.second);

	this->game->window.draw(Console::Instance());
}

void GameStateStart::update(float dt) {
	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);
	this->guiElements.at("menu")->highlight(this->guiElements.at("menu")->getBasicElement(mousePos));
}

void GameStateStart::handleInput() {
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);

	while (this->game->window.pollEvent(event)) {
		if (event.type == sf::Event::LostFocus) hasFocus = false;
		if (event.type == sf::Event::GainedFocus) hasFocus = true;

		if (!hasFocus) continue;

		switch (event.type)
		{
		case sf::Event::Closed:
		{
			game->window.close();
			break;
		}

		case sf::Event::Resized:
		{
			this->view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0),this->view));
			sf::Vector2f pos = sf::Vector2f(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
			pos *= 0.5f;
			pos = this->game->window.mapPixelToCoords(sf::Vector2i(pos), this->view);
			this->guiElements.at("menu")->setPosition(pos);
			this->game->background.setScale(
				static_cast<float>(event.size.width) / static_cast<float>(this->game->background.getTexture()->getSize().x),
				static_cast<float>(event.size.height) / static_cast<float>(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::string msg = this->guiElements.at("menu")->activate(mousePos);

				if (msg == "play_game_single")
				{
					this->playGameSingle();
				}
				if (msg == "play_game_server")
				{
					this->playGameServer();
				}
				if (msg == "play_game_client")
				{
					this->playGameClient();
				}
				if (msg == "load_game")
				{
					this->loadEditor();
				}
				if (msg == "exit_game")
				{
					this->game->window.close();
				}
			}
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) this->game->window.close();
			break;
		}
		default:
			break;
		}
		Console::Instance().HandleEvent(event);
	}
}

bool GameStateStart::end() 
{
	return this->isDeletable;
}

void GameStateStart::loadEditor() 
{
	this->game->pushState(new GameStateEditor(this->game));
	this->isDeletable = true;
}

void GameStateStart::playGameSingle()
{
	this->game->networkmgr.setType("single");
	this->game->pushState(new GameStatePlaying(this->game, new MKInput(this->game)));
	this->isDeletable = true;
}

void GameStateStart::playGameServer()
{
	this->game->networkmgr.setType("server");	
	this->game->pushState(new GameStatePlaying(this->game, new MKInput(this->game)));
	this->isDeletable = true;
}

void GameStateStart::playGameClient()
{
	this->game->networkmgr.setType("client");
	this->game->pushState(new GameStateLoading(this->game));
	//this->game->pushState(new GameStatePlaying(this->game, new MKInput(this->game)));
	this->isDeletable = true;
}

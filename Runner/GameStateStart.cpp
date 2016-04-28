#include "stdafx.h"
#include "GameStateStart.h"
#include "GameStateEditor.h"
#include "GameStatePlaying.h"

#include <utility>



GameStateStart::GameStateStart(Game* game) : shouldEnd(false)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, this->game->window.getSize().x, this->game->window.getSize().y);
	this->view.reset(viewRect);
	pos *= 0.5f;
	this->view.setCenter(pos);

	this->setGuiSystem();

}

void GameStateStart::setGuiSystem()
{
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(this->game->window.getSize().x) / float(this->game->background.getTexture()->getSize().x),
		float(this->game->window.getSize().y) / float(this->game->background.getTexture()->getSize().y));

	this->guiSystem.emplace("menu", GUI(sf::Vector2f(GAME_MENU_BUTTON_WIDTH, GAME_MENU_BUTTON_HEIGHT), 4, false, game->styleSheets.at("button"), { std::make_pair("Play","play_game"), std::make_pair("Load Editor", "load_game"), std::make_pair("Exit Game", "exit_game") }));
	this->guiSystem.at("menu").setOrigin(GAME_MENU_BUTTON_WIDTH / 2, GAME_MENU_BUTTON_HEIGHT / 2);
	this->guiSystem.at("menu").setPosition(sf::Vector2f(this->game->window.getSize().x, this->game->window.getSize().y) * 0.5f);
	this->guiSystem.at("menu").show();
}

void GameStateStart::resize(sf::Event& event)
{
	sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
	sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);

	this->view.reset(viewRect);
	this->view.setCenter(pos*0.5f);

	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
		float(event.size.height) / float(this->game->background.getTexture()->getSize().y));

	this->guiSystem.at("menu").setPosition(pos * 0.5f);
	this->guiSystem.at("menu").show();
}


GameStateStart::~GameStateStart()
{
}

void GameStateStart::draw(float dt) {
	this->game->window.setView(this->view);

	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);

	for (auto gui : guiSystem) this->game->window.draw(gui.second);
}

void GameStateStart::update(float dt) {
	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);
	this->guiSystem.at("menu").highlight(this->guiSystem.at("menu").getEntry(mousePos));
}

void GameStateStart::handleInput() {
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);

	while (this->game->window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			game->window.close();
			break;
		}

		case sf::Event::Resized:
		{
			this->view.setSize(event.size.width, event.size.height);
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0),this->view));
			sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
			pos *= 0.5f;
			pos = this->game->window.mapPixelToCoords(sf::Vector2i(pos), this->view);
			this->guiSystem.at("menu").setPosition(pos);
			this->game->background.setScale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::string msg = this->guiSystem.at("menu").activate(mousePos);

				if (msg == "play_game")
				{
					this->playGame();
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
	}
}

bool GameStateStart::end() 
{
	return this->shouldEnd;
}

void GameStateStart::loadEditor() 
{
	this->game->pushState(new GameStateEditor(this->game));
	this->shouldEnd = true;
}

void GameStateStart::playGame()
{
	this->game->pushState(new GameStatePlaying(this->game,new MKInput(this->game)));
	this->shouldEnd = true;
}
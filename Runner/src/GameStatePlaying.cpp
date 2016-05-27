#include "stdafx.h"
#include "GameStatePlaying.h"
#include "MapHitbox.h"

#include <algorithm>
#include <memory>
#include <unordered_set>
#include <omp.h>

GameStatePlaying::GameStatePlaying(Game* game, IPlayerInput* playerInput) : map(game), zoomLevel(1.0f), actionState(ActionState::NONE), isDeletable(false), cursor(game, playerInput), gameLogicManager(game)
{
	this->game = game;
	this->playerInput = playerInput;

	this->player = std::make_shared<Player>(game, playerInput);

	if (this->game->networkmgr.type == "client")
	{
		this->map.loadFromDiskPlaying(GAME_MAP_NAME_REMOTE, this->game->tileAtlas);
	}
	else
	{
		this->map.loadFromDiskPlaying(GAME_MAP_NAME, this->game->tileAtlas);
	}

	this->gameLogicManager.Init(player, &map);
	this->gameLogicManager.players.Add(player->entityID, player);
	
	this->game->networkmgr.AddGameEntity(player->entityID);
	
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, this->game->window.getSize().x, this->game->window.getSize().y);
	this->guiView.reset(viewRect);
	this->gameView.reset(viewRect);
	pos *= 0.5f;
	this->guiView.setCenter(pos);	

	this->setGuiSystem();

	sf::Vector2f center(this->map.width * 0.5, this->map.height * 0.5);
	center *= float(this->map.tileSize);
	gameView.setCenter(center);
	player->setPosition(center);

	this->game->window.setMouseCursorVisible(false);
}

void GameStatePlaying::setGuiSystem()
{
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(this->game->window.getSize().x) / float(this->game->background.getTexture()->getSize().x),
		float(this->game->window.getSize().y) / float(this->game->background.getTexture()->getSize().y));

	std::vector<std::pair<std::string, std::string>> F10 = {
		std::make_pair("F10","F10")
	};

	std::vector<std::pair<std::string, std::string>> settingsEntries = {
		std::make_pair("Resume","resume"),
		std::make_pair("Quit to Menu", "quit_to_menu"),
		std::make_pair("Exit Game","exit_game"),
	};

	this->guiSystem.emplace("F10", GUI(sf::Vector2f(64, 32), 4, true, this->game->styleSheets.at("button"), F10));
	this->guiSystem.at("F10").setPosition(sf::Vector2f(this->game->window.getSize().x - 64, 0));
	this->guiSystem.at("F10").show();

	this->guiSystem.emplace("settings", GUI(sf::Vector2f(GAME_MENU_BUTTON_WIDTH * 2, GAME_MENU_BUTTON_HEIGHT), 4, false, this->game->styleSheets.at("button"), settingsEntries));
	this->guiSystem.at("settings").setOrigin(GAME_MENU_BUTTON_WIDTH / 2, GAME_MENU_BUTTON_HEIGHT / 2);
	this->guiSystem.at("settings").setPosition(sf::Vector2f(this->game->window.getSize().x * 0.5f, this->game->window.getSize().y * 0.5f));
}

void GameStatePlaying::resize(sf::Event& event)
{
	sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
	sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);

	this->guiView.reset(viewRect);
	this->guiView.setCenter(pos*0.5f);

	this->gameView.reset(viewRect);
	this->gameView.zoom(zoomLevel);
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
		float(event.size.height) / float(this->game->background.getTexture()->getSize().y));

	this->guiSystem.at("F10").setPosition(sf::Vector2f(event.size.width - 64, 0));
	this->guiSystem.at("F10").show();
	this->guiSystem.at("settings").setPosition(sf::Vector2f(this->game->window.getSize().x * 0.5f, this->game->window.getSize().y * 0.5f));
	this->guiSystem.at("settings").hide();
}


GameStatePlaying::~GameStatePlaying()
{
	this->game->window.setMouseCursorVisible(true);
}

void GameStatePlaying::draw(float dt) {
	this->game->window.clear(sf::Color::Black);

	this->game->window.setView(this->guiView);
	this->game->window.draw(this->game->background);

	this->game->window.setView(this->gameView);

	this->map.draw(this->game->window, dt);
	this->gameLogicManager.draw(this->game->window);

	this->game->window.setView(this->guiView);
	for (auto& gui : guiSystem) this->game->window.draw(gui.second);
	this->cursor.draw(this->game->window);

	this->game->window.draw(Console::Instance());
}

bool GameStatePlaying::end() {
	return this->isDeletable;
}

void GameStatePlaying::update(float dt) {
	this->cursor.update(dt);	

	this->gameLogicManager.update(dt);
	
	// UPDATE VIEW
	this->gameView.setCenter(this->player->getPosition());
	
	this->guiSystem.at("settings").highlight(this->guiSystem.at("settings").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	this->guiSystem.at("F10").highlight(this->guiSystem.at("F10").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));

	if (aliveTimer > 0)
	{
		aliveTimer -= dt;
	}
	else
	{
		this->game->networkmgr.broadcastAlive();
		aliveTimer = 5;
	}

	// FINAL ACTIONS
	if (this->player->isDead == true)
	{
		sf::Vector2f center(this->map.width * 0.5, this->map.height * 0.5);
		center *= float(this->map.tileSize);
		this->player->isDead = false;
		this->player->setPosition(center);
		this->player->hitpoints = 100;
	}
}




void GameStatePlaying::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePosGUI = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView);
	sf::Vector2f mousePosGame = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);

	while (this->game->window.pollEvent(event))
	{
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
			this->resize(event);
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState == ActionState::NONE)
				{
					std::string msgF10 = this->guiSystem.at("F10").activate(mousePosGUI);
					if (msgF10 == "F10")
					{
						this->actionState = ActionState::SETTINGS;
						this->guiSystem.at("F10").hide();
						this->guiSystem.at("settings").show();
					}			
				}
				else if (this->actionState == ActionState::SETTINGS)
				{
					std::string msg = this->guiSystem.at("settings").activate(mousePosGUI);
					if (msg == "resume") {
						this->actionState = ActionState::NONE;
						this->guiSystem.at("settings").hide();
						this->guiSystem.at("F10").show();
					}
					else if (msg == "quit_to_menu")
					{
						this->isDeletable = true;
					}
					else if (msg == "exit_game")
					{
						this->game->window.close();
					}
				}

			}
			break;
		}
		case sf::Event::MouseWheelScrolled:
		{
			if (event.mouseWheelScroll.delta < 0)
			{
				gameView.zoom(2.0f);
				zoomLevel *= 2.0f;
			}
			else
			{
				gameView.zoom(0.5f);
				zoomLevel *= 0.5f;
			}
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::F10)
			{
				if (this->actionState == ActionState::NONE)
				{
					this->actionState = ActionState::SETTINGS;
					this->guiSystem.at("settings").show();
					this->guiSystem.at("F10").hide();
				}
				else if (this->actionState == ActionState::SETTINGS)
				{
					this->actionState = ActionState::NONE;
					this->guiSystem.at("settings").hide();
					this->guiSystem.at("F10").show();
				}
			}
			if (event.key.code == sf::Keyboard::F4)
			{
				sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView));
				GuidGenerator gen;
				gameLogicManager.enemies.Add(gen.newGuid(), std::make_unique<Enemies>(game, mousePos));
			}

			if (event.key.code == sf::Keyboard::F9)
			{
				this->player->setPosition(sf::Vector2f(0, 0));
			}

			//if (event.key.code == sf::Keyboard::LAlt && event.key.code == sf::Keyboard::Period)

			//if(event.key.code == sf::Keyboard::)
			//{
			//	Console::Instance(game).ToggleShow();
			//}

		}
		default:
			break;
		}

		Console::Instance().HandleEvent(event);
	}

	if (!hasFocus) return;
	if (Console::Instance().getShow())
		return;
	this->playerInput->handleInput();
}
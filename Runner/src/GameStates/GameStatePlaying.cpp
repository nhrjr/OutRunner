#include "stdafx.h"
#include "GameStatePlaying.h"
#include "Map/MapHitbox.h"

#include <algorithm>
#include <memory>
#include <unordered_set>
#include <omp.h>
#include "Network/NetworkManager.h"

#include "Core/CycleCounter.h"

GameStatePlaying::GameStatePlaying(Game* game, IPlayerInput* playerInput) : map(game), zoomLevel(1.0f), actionState(ActionState::NONE), isDeletable(false), cursor(game, playerInput), gameLogicManager(game), gui(game)/*, stateMachine(StateMachine::create())*/
{
	this->game = game;
	
	this->playerInput = playerInput;

	this->player = std::make_shared<Player>(game, playerInput, false);

	if (this->game->networkmgr->type == "client")
	{
		this->map.loadFromDiskPlaying(GAME_MAP_NAME_REMOTE, this->game->tileAtlas);
	}
	else
	{
		this->map.loadFromDiskPlaying(GAME_MAP_NAME, this->game->tileAtlas);
	}

	this->gameLogicManager.Init(player, &map);
	
	this->game->networkmgr->AddGameEntity(player->entityID);
	
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, static_cast<float>(this->game->window.getSize().x), static_cast<float>(this->game->window.getSize().y));
	this->guiView.reset(viewRect);
	this->gameView.reset(viewRect);
	pos *= 0.5f;
	this->guiView.setCenter(pos);	

	this->setGameGUI();

	sf::Vector2f center(static_cast<float>(this->map.width) * 0.5f, static_cast<float>(this->map.height) * 0.5f);
	center *= float(this->map.tileSize);
	gameView.setCenter(center);
	player->setPosition(center);

	this->game->window.setMouseCursorVisible(false);

	/*stateMachine = std::make_unique<StateMachine>();
	stateMachine->addState<LowState>();
	stateMachine->addState<HighState>();
	stateMachine->enterState<LowState>();

	Command c = [this](std::vector<std::string> str) -> std::string {
		this->stateMachine->enterState<LowState>();
		return "Command: LowState";
	};
	Console::Instance().Add("LowState", c);

	c = [this](std::vector<std::string> str) -> std::string {
		this->stateMachine->enterState<HighState>();
		return "Command: HighState";
	};
	Console::Instance().Add("HighState", c);*/
}

void GameStatePlaying::setGameGUI()
{
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(this->game->window.getSize().x) / float(this->game->background.getTexture()->getSize().x),
		float(this->game->window.getSize().y) / float(this->game->background.getTexture()->getSize().y));

	this->player->changedHitpoints.connect<GuiPlaying,&GuiPlaying::setPlayerHitpoints>(&gui);
	this->player->changedWeapon.connect<GuiPlaying, &GuiPlaying::setPlayerWeapon>(&gui);
	this->player->changedAmmo.connect<GuiPlaying, &GuiPlaying::setPlayerAmmo>(&gui);

	this->gui.setPlayerWeapon(this->player->weapon->get()->name);
}

void GameStatePlaying::resize(sf::Event& event)
{
	sf::Vector2f pos = sf::Vector2f(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
	sf::FloatRect viewRect(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));

	this->guiView.reset(viewRect);
	this->guiView.setCenter(pos*0.5f);

	this->gameView.reset(viewRect);
	this->gameView.zoom(zoomLevel);
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		static_cast<float>(event.size.width) / static_cast<float>(this->game->background.getTexture()->getSize().x),
		static_cast<float>(event.size.height) / static_cast<float>(this->game->background.getTexture()->getSize().y));

	this->gui.resize();
	
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

	this->game->window.draw(gui);

	//
	this->cursor.draw(this->game->window);

	this->game->window.draw(Console::Instance());
}

bool GameStatePlaying::end() {
	return this->isDeletable;
}

void GameStatePlaying::update(float dt) {
	//this->stateMachine->updateWithDeltaTime(dt);
	this->cursor.update(dt);	

	this->gameLogicManager.update(dt);
	
	// UPDATE VIEW
	this->gameView.setCenter(this->player->getPosition());
	sf::Vector2f mouse = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView);
	this->gui.highlight(mouse);
	
	if (aliveTimer > 0)
	{
		aliveTimer -= dt;
	}
	else
	{
		this->game->networkmgr->broadcastAlive();
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
				std::string msg = this->gui.activate(mousePosGUI);
				if (this->actionState == ActionState::NONE)
				{
					if (msg == "F10")
					{
						this->actionState = ActionState::SETTINGS;
						this->gui.guiElements.at("F10")->hide();
						this->gui.guiElements.at("settings")->show();
					}			
				}
				else if (this->actionState == ActionState::SETTINGS)
				{
					if (msg == "resume") {
						this->actionState = ActionState::NONE;
						this->gui.guiElements.at("settings")->hide();
						this->gui.guiElements.at("F10")->show();
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
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::F10)
			{
				if (this->actionState == ActionState::NONE)
				{
					this->actionState = ActionState::SETTINGS;
					this->gui.guiElements.at("settings")->show();
					this->gui.guiElements.at("F10")->hide();
				}
				else if (this->actionState == ActionState::SETTINGS)
				{
					this->actionState = ActionState::NONE;
					this->gui.guiElements.at("settings")->hide();
					this->gui.guiElements.at("F10")->show();
				}
			}
			if (event.key.code == sf::Keyboard::F4)
			{
				//sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView));
				GuidGenerator gen;
				gameLogicManager.enemies.Add(gen.newGuid(), std::make_unique<Enemies>(game, mousePosGame));
			}

			if (event.key.code == sf::Keyboard::F9)
			{
				this->player->setPosition(sf::Vector2f(0, 0));
			}
		}
		default:
			break;
		}

		this->playerInput->handleEvent(event);
		Console::Instance().HandleEvent(event);
	}

	if (!hasFocus) return;
	if (Console::Instance().getShow())
		return;
	this->playerInput->handleInput();
}
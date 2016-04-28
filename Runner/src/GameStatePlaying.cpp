#include "stdafx.h"
#include "GameStatePlaying.h"
#include "MapHitbox.h"

#include <algorithm>
#include <unordered_set>
#include <omp.h>

GameStatePlaying::GameStatePlaying(Game* game,IPlayerInput* playerInput) : map(game), zoomLevel(1.0f), actionState(ActionState::NONE), shouldEnd(false), player(game,playerInput), cursor(game,playerInput)
{
	this->game = game;
	this->playerInput = playerInput;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, this->game->window.getSize().x, this->game->window.getSize().y);
	this->guiView.reset(viewRect);
	this->gameView.reset(viewRect);
	pos *= 0.5f;
	this->guiView.setCenter(pos);

	this->map.loadFromDiskPlaying("data/map.dat", this->game->tileAtlas);

	this->setGuiSystem();

	sf::Vector2f center(this->map.width * 0.5, this->map.height * 0.5);
	center *= float(this->map.tileSize);
	gameView.setCenter(center);
	player.setPosition(center);
	//npc.setPosition(center - sf::Vector2f(2 * PLAYER_RADIUS, 0));

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
	this->player.draw(this->game->window, dt);
	//this->npc.draw(this->game->window, dt);

	if (this->game->switches.showPath)
	{
		this->drawTestPath();
	}
	for (auto& obj : enemies) obj.draw(this->game->window, dt);
	for (auto& obj : bullets) obj.draw(this->game->window, dt);
	

	this->game->window.setView(this->guiView);
	for (auto& gui : guiSystem) this->game->window.draw(gui.second);
	this->cursor.draw(this->game->window, dt);

	
}

bool GameStatePlaying::end() {
	return this->shouldEnd;
}

void GameStatePlaying::update(float dt) {
	// CURSOR
	this->cursor.update(dt);

	// COLLISION PLAYER WITH MAP
	this->player.update(dt);
	std::unordered_set<unsigned int> indices = map.grid.getCollisionObjects(this->player.getPosition());
	//map.setDrawableHitboxes(indices);
	std::unordered_set<unsigned int>::const_iterator it;
	for(it = indices.begin(); it != indices.end(); ++it)
	{
		this->player.collide(map.hitboxes[*it], 0, dt);
	}
	for (auto& bullet : bullets) {
		player.collide(bullet, 1, dt);
	}
	if (this->player.isShooting) bullets.emplace_back(Bullet(this->game, this->player.attackingAngle, this->player.getPosition()));

	// COLLISION NPC WITH MAP
	//if (this->player.isAttacking)
	//{
	//	sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView));
	//	npc.shootAtTarget(mousePos);
	//}
	//else
	//{
	//	this->npc.isAttacking = false;
	//}
	//this->npc.update(dt);
	//indices = map.grid.getCollisionObjects(this->npc.getPosition());
	//map.setDrawableHitboxes(indices);
	//for (it = indices.begin(); it != indices.end(); ++it)
	//{
	//	this->npc.collide(map.hitboxes[*it], 0, dt);
	//}

	for (auto& enemy : enemies)
	{
		if (enemy.targetList.empty())
		{
			this->testPath = this->map.pathfinder.searchPath(enemy.getPosition(), player.getPosition());
			enemy.moveToTarget(this->testPath);
		}
		enemy.update(dt);
		for (auto& bullet : bullets) {
			enemy.collide(bullet, 1, dt);
		}

		indices = map.grid.getCollisionObjects(enemy.getPosition());
		for (it = indices.begin(); it != indices.end(); ++it)
		{
			enemy.collide(map.hitboxes[*it], 0, dt);
		}
		
		if (V2Tools::inLineOfSight_againstPolygon(this->map.polygons, enemy.getPosition(), player.getPosition()))
		{
			enemy.shootAtTarget(player.getPosition());
		}
		else
		{
			enemy.isAttacking = false;
		}
		if (enemy.isShooting) bullets.emplace_back(Bullet(this->game, enemy.attackingAngle, enemy.getPosition()));
	}
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const IGameEntity& obj) {return obj.shouldEnd; }), enemies.end());

	// COLLISION BULLETS WITH MAP
	
	//if (this->npc.isShooting) bullets.emplace_back(Bullet(this->game, this->npc.attackingAngle, this->npc.getPosition()));
	for (auto& bullet : bullets)
	{
		bullet.update(dt);

		//this->player.collide(bullet, 1, dt);
		//this->npc.collide(bullet, 1, dt);

		indices = map.grid.getCollisionObjects(bullet.getPosition());
		for (it = indices.begin(); it != indices.end(); ++it)
		{
			bullet.collide(map.hitboxes[*it], 0, dt);
		}
	}
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const IGameEntity& obj) {return obj.shouldEnd; }), bullets.end());



	// UPDATE VIEW
	this->gameView.setCenter(this->player.getPosition());
	
	this->guiSystem.at("settings").highlight(this->guiSystem.at("settings").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	this->guiSystem.at("F10").highlight(this->guiSystem.at("F10").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));

	// FINAL ACTIONS
	if (this->player.shouldEnd == true)
	{
		this->shouldEnd = true;
	}
		
}




void GameStatePlaying::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePosGUI = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView);
	sf::Vector2f mousePosGame = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);

	while (this->game->window.pollEvent(event))
	{
		switch (event.type)
		{
		default:
			break;
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
		//case sf::Event::MouseMoved:
		//{

		//	if (this->actionState == ActionState::PANNING)
		//	{
		//		sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
		//		gameView.move(-1.0f * position * this->zoomLevel);
		//		panningAnchor = sf::Mouse::getPosition(this->game->window);
		//	}
		//	break;
		//}
		case sf::Event::MouseButtonPressed:
		{
			//if (event.mouseButton.button == sf::Mouse::Middle)
			//{
			//	if (this->actionState != ActionState::PANNING)
			//	{
			//		this->actionState = ActionState::PANNING;
			//		this->panningAnchor = sf::Mouse::getPosition(this->game->window);
			//	}
			//}
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
					//else if (msg == "save_map")
					//{
					//	this->actionState = ActionState::NONE;
					//	//this->map.saveToDisk("data/map.dat");
					//	this->guiSystem.at("settings").hide();
					//	this->guiSystem.at("F10").show();
					//}
					else if (msg == "quit_to_menu")
					{
						this->shouldEnd = true;
					}
					else if (msg == "exit_game")
					{
						this->game->window.close();
					}
				}

			}
			//if (event.mouseButton.button == sf::Mouse::Right)
			//{
			//	testPathfinder();
			//	this->npc.moveToTarget(testPath);
			//}
			break;
		}
		//case sf::Event::MouseButtonReleased:
		//{
		//	if (event.mouseButton.button == sf::Mouse::Middle)
		//	{
		//		this->actionState = ActionState::NONE;
		//	}
		//	break;
		//}
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
			if (event.key.code == sf::Keyboard::F5)
			{
				this->game->switches.showPath = (this->game->switches.showPath == true) ? false : true;
			}
			if (event.key.code == sf::Keyboard::F6)
			{
				this->game->switches.showPathfinder = (this->game->switches.showPathfinder == true) ? false : true;
			}
			if (event.key.code == sf::Keyboard::F7)
			{
				this->game->switches.showHitboxHighlights = (this->game->switches.showHitboxHighlights == true) ? false : true;
			}
			if (event.key.code == sf::Keyboard::F8)
			{
				this->game->switches.showPolygon = (this->game->switches.showPolygon == true) ? false : true;
			}
			if (event.key.code == sf::Keyboard::F9)
			{
				this->player.setPosition(sf::Vector2f(0, 0));
			}
			if (event.key.code == sf::Keyboard::E)
			{
				sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window),this->gameView));
				//npc.shootAtTarget(mousePos);
				this->enemies.emplace_back(Enemies(game,mousePos));

			}
		}
		}

	}
}

//void GameStatePlaying::testPathfinder()
//{
//	sf::Vector2f start = this->npc.getPosition();
//	
//	sf::Vector2f end = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
//	
//	testPath = this->map.pathfinder.searchPath(start, end);
//}

void GameStatePlaying::drawTestPath()
{
	
	int i = 0;
	for(std::deque<sf::Vector2f>::const_iterator it = testPath.begin(); it != testPath.end(); ++it){
	//for(auto point : testPath){
		sf::Text text(std::to_string(i++), this->game->fonts["main_font"], 12);
		text.setPosition(*it);
		sf::CircleShape circle(5);
		circle.setOrigin(5, 5);
		circle.setFillColor(sf::Color::Black);
		circle.setPosition(*it);
		this->game->window.draw(circle);
		this->game->window.draw(text);
	}

	
}
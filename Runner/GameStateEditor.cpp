#include "stdafx.h"
#include "GameStateEditor.h"

#include <type_traits> //for std::underlying_type
template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}


GameStateEditor::GameStateEditor(Game* game) : map(game), zoomLevel(1.0f), actionState(ActionState::NONE), editingState(EditionState::NONE), shouldEnd(false), editingTileType(TileType::VOID), currentTile(nullptr)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	sf::FloatRect viewRect(0, 0, this->game->window.getSize().x, this->game->window.getSize().y);
	this->guiView.reset(viewRect);
	this->guiView.setCenter(pos * 0.5f);

	this->setGuiSystem();

	//this->map = Map();
	this->map.loadFromDiskEditor("data/map.dat", this->game->tileAtlas);

	sf::Vector2f center(this->map.width * 0.5, this->map.height * 0.5);
	center *= float(this->map.tileSize);
	this->gameView.reset(viewRect);
	this->gameView.setCenter(center);

	this->selectionRectangle.setFillColor(sf::Color(255, 255, 255, 128));
}

void GameStateEditor::setGuiSystem()
{
	this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
	this->game->background.setScale(
		float(this->game->window.getSize().x) / float(this->game->background.getTexture()->getSize().x),
		float(this->game->window.getSize().y) / float(this->game->background.getTexture()->getSize().y));

	std::vector<std::pair<std::string, std::string>> F10 = {
		std::make_pair("F10","F10")
	};

	std::vector<std::pair<std::string, std::string>> toolBarEntries = {
		std::make_pair("Concrete","tile_concrete"),
		std::make_pair("Water","tile_water"),
		std::make_pair("Wall","tile_wall")
	};

	std::vector<std::pair<std::string, std::string>> settingsEntries = {
		std::make_pair("Resume","resume"),
		std::make_pair("Save Map","save_map"),
		std::make_pair("Save & Quit", "save_quit"),
		std::make_pair("Quit to Menu","quit_to_menu"),
		std::make_pair("Exit Game","exit_game"),
	};

	this->guiSystem.clear();

	this->guiSystem.emplace("toolbar", GUI(sf::Vector2f(128, 32), 4, true, this->game->styleSheets.at("button"), toolBarEntries));
	this->guiSystem.at("toolbar").setPosition(sf::Vector2f(0, this->game->window.getSize().y - 40));
	this->guiSystem.at("toolbar").show();

	this->guiSystem.emplace("F10", GUI(sf::Vector2f(64, 32), 4, true, this->game->styleSheets.at("button"), F10));
	this->guiSystem.at("F10").setPosition(sf::Vector2f(this->game->window.getSize().x - 64, 0));
	this->guiSystem.at("F10").show();

	this->guiSystem.emplace("settings", GUI(sf::Vector2f(GAME_MENU_BUTTON_WIDTH * 2, GAME_MENU_BUTTON_HEIGHT), 4, false, this->game->styleSheets.at("button"), settingsEntries));
	this->guiSystem.at("settings").setOrigin(GAME_MENU_BUTTON_WIDTH / 2, GAME_MENU_BUTTON_HEIGHT / 2);
	this->guiSystem.at("settings").setPosition(sf::Vector2f(this->game->window.getSize().x * 0.5f, this->game->window.getSize().y * 0.5f));
}

void GameStateEditor::resize(sf::Event& event)
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

	this->guiSystem.at("toolbar").setPosition(sf::Vector2f(0, event.size.height - 40));
	this->guiSystem.at("toolbar").hide();
	this->guiSystem.at("toolbar").show();
	this->guiSystem.at("F10").setPosition(sf::Vector2f(event.size.width - 64, 0));
	this->guiSystem.at("F10").show();
	this->guiSystem.at("settings").setPosition(sf::Vector2f(this->game->window.getSize().x * 0.5f, this->game->window.getSize().y * 0.5f));
}


GameStateEditor::~GameStateEditor()
{
}

void GameStateEditor::draw(float dt) {

	this->game->window.clear(sf::Color::Black);

	this->game->window.setView(this->guiView);
	this->game->window.draw(this->game->background);

	this->game->window.setView(this->gameView);
	this->map.draw(this->game->window, dt);
	this->game->window.draw(selectionRectangle);

	this->game->window.setView(this->guiView);
	for (auto gui : guiSystem) this->game->window.draw(gui.second);
}

bool GameStateEditor::end() {
	return this->shouldEnd;
}

void GameStateEditor::update(float dt) {
	this->guiSystem.at("settings").highlight(this->guiSystem.at("settings").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	this->guiSystem.at("F10").highlight(this->guiSystem.at("F10").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));

	int mouseHighLightEntry = this->guiSystem.at("toolbar").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView));
	if (mouseHighLightEntry >= 0) {
		this->guiSystem.at("toolbar").highlight(mouseHighLightEntry);
	}
	else {
		if (this->editingTileType == TileType::CONCRETE)
		{
			this->guiSystem.at("toolbar").highlight(this->guiSystem.at("toolbar").getEntry("tile_concrete"));
		}
		else if (this->editingTileType == TileType::WATER) {
			this->guiSystem.at("toolbar").highlight(this->guiSystem.at("toolbar").getEntry("tile_water"));
		}
		else if (this->editingTileType == TileType::WALL) {
			this->guiSystem.at("toolbar").highlight(this->guiSystem.at("toolbar").getEntry("tile_wall"));
		}
	}
}

void GameStateEditor::handleInput()
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
		case sf::Event::MouseMoved:
		{
			
			if (this->actionState == ActionState::PANNING)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
				gameView.move(-1.0f * pos * this->zoomLevel);
				panningAnchor = sf::Mouse::getPosition(this->game->window);
			}
			if (this->editingState == EditionState::PAINTING) 
			{
				Tile* currentTile = map.getCurrentTile(mousePosGame);
				if (currentTile != nullptr)
				{
					if (this->editingTileType == TileType::CONCRETE)
					{
						*currentTile = this->game->tileAtlas.at("Concrete");
					}
					else if (this->editingTileType == TileType::WATER)
					{
						*currentTile = this->game->tileAtlas.at("Water");
					}
					else if (this->editingTileType == TileType::WALL)
					{
						*currentTile = this->game->tileAtlas.at("Wall");
					}
				}
			}
			if (this->editingState == EditionState::SELECTING)
			{
				selectionRectangle.setSize(mousePosGame-selectionRectangle.getPosition());
			}
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				if (this->actionState != ActionState::PANNING)
				{
					this->actionState = ActionState::PANNING;
					this->panningAnchor = sf::Mouse::getPosition(this->game->window);
				}
			}
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState == ActionState::NONE)
				{
					std::string msgF10 = this->guiSystem.at("F10").activate(mousePosGUI);
					std::string msgToolbar = this->guiSystem.at("toolbar").activate(mousePosGUI);
					if (msgF10 == "F10")
					{
						this->actionState = ActionState::SETTINGS;
						this->guiSystem.at("F10").hide();
						this->guiSystem.at("settings").show();
					}					
					else if (msgToolbar == "tile_concrete")
					{
						if (this->editingTileType != TileType::CONCRETE) {
							this->editingState = EditionState::EDITING;
							this->editingTileType = TileType::CONCRETE;
						}
						else {
							this->editingState = EditionState::NONE;
							this->editingTileType = TileType::VOID;
						}
					}
					else if(msgToolbar == "tile_water"){
						if (this->editingTileType != TileType::WATER) {
							this->editingState = EditionState::EDITING;
							this->editingTileType = TileType::WATER;
						}
						else {
							this->editingState = EditionState::NONE;
							this->editingTileType = TileType::VOID;
						}
					}
					else if (msgToolbar == "tile_wall") {
						if (this->editingTileType != TileType::WALL) {
							this->editingState = EditionState::EDITING;
							this->editingTileType = TileType::WALL;
						}
						else {
							this->editingState = EditionState::NONE;
							this->editingTileType = TileType::VOID;
						}
					}
					else if (this->editingState == EditionState::EDITING)
					{
						Tile* currentTile = map.getCurrentTile(mousePosGame);
						if (currentTile != nullptr)
						{
							if (this->editingTileType == TileType::CONCRETE)
							{
								this->editingState = EditionState::PAINTING;
								*currentTile = this->game->tileAtlas.at("Concrete");
							}
							else if (this->editingTileType == TileType::WATER)
							{
								this->editingState = EditionState::PAINTING;
								*currentTile = this->game->tileAtlas.at("Water");
							}
							else if (this->editingTileType == TileType::WALL)
							{
								this->editingState = EditionState::PAINTING;
								*currentTile = this->game->tileAtlas.at("Wall");
							}
						}
					}
				}
				else if (this->actionState == ActionState::SETTINGS)
				{
					this->editingState = EditionState::NONE;
					std::string msg = this->guiSystem.at("settings").activate(mousePosGUI);
					if (msg == "resume") {
						this->actionState = ActionState::NONE;
						this->guiSystem.at("settings").hide();
						this->guiSystem.at("F10").show();
					}
					else if (msg == "save_map")
					{
						this->actionState = ActionState::NONE;
						this->map.saveToDisk("data/map.dat");
						this->guiSystem.at("settings").hide();
						this->guiSystem.at("F10").show();
					}
					else if (msg == "save_quit")
					{
						this->map.saveToDisk("data/map.dat");
						this->shouldEnd = true;
					}
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
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (this->actionState == ActionState::NONE)
				{
					if (this->editingState == EditionState::EDITING)
					{
						this->editingState = EditionState::SELECTING;
						selectionRectangle.setPosition(mousePosGame);
					}

				}
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				this->actionState = ActionState::NONE;
			}
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if(this->editingState == EditionState::PAINTING)
					this->editingState = EditionState::EDITING;
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (this->editingState == EditionState::SELECTING)
				{
					editSelectedTiles();
					this->editingState = EditionState::EDITING;
					selectionRectangle.setSize(sf::Vector2f(0, 0));
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
		}
		}

	}
}

void GameStateEditor::editSelectedTiles()
{
	sf::FloatRect rect = selectionRectangle.getGlobalBounds();
	for (int i = rect.left ; i < rect.width + rect.left ; i += GAME_TILESIZE)
	{
		for (int j = rect.top ; j < rect.top + rect.height ; j += GAME_TILESIZE)
		{
			Tile* currentTile = map.getCurrentTile(sf::Vector2f(i,j));
			if (currentTile != nullptr)
			{
				if (this->editingTileType == TileType::CONCRETE)
				{
					*currentTile = this->game->tileAtlas.at("Concrete");
				}
				else if (this->editingTileType == TileType::WATER)
				{
					*currentTile = this->game->tileAtlas.at("Water");
				}
				else if (this->editingTileType == TileType::WALL)
				{
					*currentTile = this->game->tileAtlas.at("Wall");
				}
			}
		}
	}
		
	
}



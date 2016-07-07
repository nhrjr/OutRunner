#pragma once
#include "stdafx.h"
#include "IGameState.h"
#include "Map/Map.h"
#include "Gui/GuiList.h"

enum class EditionState { NONE, EDITING, PAINTING, SELECTING };

class GameStateEditor :
	public IGameState
{
	bool isDeletable;
	bool hasFocus = true;

	ActionState actionState;
	EditionState editingState;

	sf::View gameView;
	sf::View guiView;

	sf::Vector2i panningAnchor;
	float zoomLevel;

	std::map<std::string, std::shared_ptr<GuiElement>> guiElements;

	TileType editingTileType;
	Tile* currentTile;

	Map map;

	sf::RectangleShape selectionRectangle;
public:
	GameStateEditor(Game* game);
	~GameStateEditor();

	virtual void update(float dt);
	virtual void draw(float dt);
	virtual void handleInput();
	virtual bool end();

	void setGameGUI();
	void resize(sf::Event& event);

private:
	void editSelectedTiles();
};


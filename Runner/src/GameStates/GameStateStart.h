#pragma once
#include "stdafx.h"
#include "IGameState.h"
#include "Gui/GuiList.h"

class GameStateStart :
	public IGameState
{
public:
	GameStateStart(Game* game);
	~GameStateStart();

	virtual void update(float dt);
	virtual void draw(float dt);
	virtual void handleInput();
	virtual bool end();

	void setGameGUI();
	void resize(sf::Event& event);

private: 
	bool isDeletable;
	bool hasFocus = true;
	sf::View view;
	std::map<std::string, std::shared_ptr<GuiElement>> guiElements;
	void loadEditor();
	void playGameSingle();
	void playGameServer();
	void playGameClient();
};


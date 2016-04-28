#pragma once
#include "stdafx.h"
#include "IGameState.h"
#include "GUI.h"
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

	void setGuiSystem();
	void resize(sf::Event& event);

private: 
	bool shouldEnd;
	sf::View view;
	std::map<std::string, GUI> guiSystem;
	void loadEditor();
	void playGame();
};


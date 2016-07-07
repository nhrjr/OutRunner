#pragma once
#include "IGameState.h"
class GameStateLoading :
	public IGameState
{
public:
	GameStateLoading(Game* game);
	~GameStateLoading();

	virtual void update(float dt);
	virtual void draw(float dt);
	virtual void handleInput();
	virtual bool end();

	void setGameGUI();
	void resize(sf::Event& event);

private:
	bool isDeletable;
	sf::View view;
	//std::map<std::string, GuiList> guiElements;

	float temp_FindServerTimer = 1000;
};


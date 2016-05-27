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

	void setGuiSystem();
	void resize(sf::Event& event);

private:
	bool isDeletable;
	sf::View view;
	std::map<std::string, GUI> guiSystem;

	float temp_FindServerTimer = 1000;
};


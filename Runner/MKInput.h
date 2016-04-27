#pragma once
#include "Game.h"
#include "IPlayerInput.h"

class MKInput : public IPlayerInput
{
public:
	MKInput(Game* game);
	~MKInput();

	virtual float getAngle();
	virtual sf::Vector2f getOffset();
	virtual int getAction();
	virtual int getAlternateAction();

private:
	Game* game;

	sf::Vector2f up;
	sf::Vector2f down;
	sf::Vector2f left;
	sf::Vector2f right;
};


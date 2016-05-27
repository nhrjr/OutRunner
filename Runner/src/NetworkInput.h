#pragma once
#include "Game.h"
#include "IPlayerInput.h"

class NetworkInput : public IPlayerInput
{
public:
	NetworkInput(Game* game);
	~NetworkInput();

	virtual void getInput(float dt, const std::stack<NetworkPlayerEvent>& events);

	virtual float getAngle();
	virtual sf::Vector2f getPosition(sf::Vector2f lastPos);
	virtual int getAction();
	virtual int getAlternateAction();
	virtual void handleInput();

private:
	Game* game;

	sf::Vector2f up;
	sf::Vector2f down;
	sf::Vector2f left;
	sf::Vector2f right;

	sf::Vector2f networkPos;
	float networkAngle;
	int action;
	int alternateAction;
};
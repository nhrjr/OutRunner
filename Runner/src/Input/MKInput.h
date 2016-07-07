#pragma once
#include "Core/Game.h"
#include "IPlayerInput.h"

class MKInput : public IPlayerInput
{
public:
	MKInput(Game* game);
	~MKInput();

	virtual void getInput(float dt);

	virtual float getAngle();
	virtual sf::Vector2f getPosition(sf::Vector2f lastPos);
	virtual int getAction();
	virtual int getAlternateAction();
	virtual void handleInput();
	virtual void handleEvent(const sf::Event& event);
	virtual std::vector<int> getEvent();

private:
	Game* game;

	sf::Vector2f up;
	sf::Vector2f down;
	sf::Vector2f left;
	sf::Vector2f right;
	sf::Vector2f directionOffset;
	float angle;
	float dt;

	int action;
	int alternateAction;
	std::vector<int> events;
};


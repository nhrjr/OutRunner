#include "stdafx.h"
#include "NetworkInput.h"

#include <cmath>


NetworkInput::NetworkInput(Game* game)
{
	this->game = game;
	this->up = sf::Vector2f(0, -1);
	this->down = sf::Vector2f(0, 1);
	this->right = sf::Vector2f(1, 0);
	this->left = sf::Vector2f(-1, 0);
}

void NetworkInput::getInput(float dt,const std::stack<NetworkPlayerEvent>& events)
{
	if (events.size() != 0)
	{
		networkPos = sf::Vector2f(events.top().x, events.top().y);
		networkAngle = events.top().angle;
		action = events.top().action;
		alternateAction = events.top().alternateAction;
	}
	else {
		action = 0;
		alternateAction = 0;
	}
}

void NetworkInput::handleInput()
{

}

void NetworkInput::handleEvent(const sf::Event& event)
{

}

std::vector<int> NetworkInput::getEvent()
{
	std::vector<int> tmp;
	return tmp;
}


float NetworkInput::getAngle()
{
	return networkAngle;
}

sf::Vector2f NetworkInput::getPosition(sf::Vector2f lastPos) {
	return networkPos;
}

int NetworkInput::getAction()
{
	return action;
}

int NetworkInput::getAlternateAction()
{
	return alternateAction;
}

NetworkInput::~NetworkInput()
{
}

#include "stdafx.h"
#include "MKInput.h"

#include <cmath>


MKInput::MKInput(Game* game)
{
	this->game = game;
	this->up = sf::Vector2f(0, -1);
	this->down = sf::Vector2f(0, 1);
	this->right = sf::Vector2f(1, 0);
	this->left = sf::Vector2f(-1, 0);
}

MKInput::~MKInput()
{
}

void MKInput::getInput(float dt)
{
	this->dt = dt;
}

float MKInput::getAngle()
{
	//sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window)));
	//sf::Vector2f direction = mousePos - sf::Vector2f(this->game->window.getSize().x / 2, this->game->window.getSize().y / 2);
	//float angleOffset = atan2(direction.y, direction.x) * 180 / M_PI;
	return (float)fmod(this->angle, 360);
}

void MKInput::handleInput()
{
	this->directionOffset = sf::Vector2f(0, 0);
	int count = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->directionOffset += up;
		++count;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->directionOffset += down;
		++count;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->directionOffset += left;
		++count;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->directionOffset += right;
		++count;
	}
	if (count >= 2) {
		this->directionOffset /= sqrtf(2);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		action = 1;
	}
	else {
		action = 0;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		alternateAction = 1;
	}
	else {
		alternateAction = 0;
	}

	sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window)));
	sf::Vector2f direction = mousePos - sf::Vector2f(static_cast<float>(this->game->window.getSize().x / 2), static_cast<float>(this->game->window.getSize().y / 2));
	this->angle = atan2(direction.y, direction.x) * 180.f / M_PI;
}

void MKInput::handleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::MouseWheelScrolled:
	{
		if (ev.mouseWheelScroll.delta < 0)
		{
			events.push_back(0);
		}
		else
		{
			events.push_back(1);
		}
		break;
	}
	case sf::Event::KeyPressed:
	{
		if (ev.key.code == sf::Keyboard::Q) events.push_back(2);
		if (ev.key.code == sf::Keyboard::E) events.push_back(3);
		if (ev.key.code == sf::Keyboard::R) events.push_back(4);
		if (ev.key.code == sf::Keyboard::F) events.push_back(5);
		break;
	}
	}
}

sf::Vector2f MKInput::getPosition(sf::Vector2f lastPos) {
	
	float moveBy = PLAYER_SPEED * dt;
	return lastPos + directionOffset * moveBy;
}

int MKInput::getAction()
{
	return action;
}

int MKInput::getAlternateAction()
{
	return alternateAction;
}

std::vector<int> MKInput::getEvent()
{
	return events;
}




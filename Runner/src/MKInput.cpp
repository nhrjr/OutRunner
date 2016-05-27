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

void MKInput::getInput(float dt)
{
	this->dt = dt;
}

float MKInput::getAngle()
{
	sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window)));
	sf::Vector2f direction = mousePos - sf::Vector2f(this->game->window.getSize().x / 2, this->game->window.getSize().y / 2);
	float angleOffset = atan2(direction.y, direction.x) * 180 / M_PI;
	return angleOffset = fmod(angleOffset, 360);
}

void MKInput::handleInput()
{
	this->directionOffset = sf::Vector2f(0, 0);
	int count = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->directionOffset += up;
		count++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->directionOffset += down;
		count++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->directionOffset += left;
		count++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->directionOffset += right;
		count++;
	}
	if (count >= 2) {
		this->directionOffset /= sqrtf(2);
	}
}

sf::Vector2f MKInput::getPosition(sf::Vector2f lastPos) {
	
	float moveBy = PLAYER_SPEED * dt;
	return lastPos + directionOffset * moveBy;
}

int MKInput::getAction()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		return 1;
	}
	return 0;
}

int MKInput::getAlternateAction()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		return 1;
	}
	return 0;
}


MKInput::~MKInput()
{
}

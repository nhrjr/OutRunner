#include "stdafx.h"
#include "MKInput.h"


MKInput::MKInput(Game* game)
{
	this->game = game;
	this->up = sf::Vector2f(0, -1);
	this->down = sf::Vector2f(0, 1);
	this->right = sf::Vector2f(1, 0);
	this->left = sf::Vector2f(-1, 0);
	

	//sf::Vector2f forward;

	//forward.x = cos(shape.getRotation() * M_PI / 180);
	//forward.y = sin(shape.getRotation() * M_PI / 180);

	//sf::Vector2f back = -forward;
	//sf::Vector2f right = sf::Vector2f(-forward.y, forward.x);
	//sf::Vector2f left = sf::Vector2f(forward.y, - forward.x);
}

float MKInput::getAngle()
{
	sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window)));
	sf::Vector2f direction = mousePos - sf::Vector2f(this->game->window.getSize().x / 2, this->game->window.getSize().y / 2);
	float angleOffset = atan2(direction.y, direction.x) * 180 / M_PI;
	return angleOffset = fmod(angleOffset, 360);
}

sf::Vector2f MKInput::getOffset() {
	sf::Vector2f directionOffset = sf::Vector2f(0, 0);

	int count = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		directionOffset += up;
		count++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		directionOffset += down;
		count++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		directionOffset += left;
		count++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		directionOffset += right;
		count++;
	}
	if (count >= 2) {
		directionOffset /= sqrtf(2);
	}
	return directionOffset;
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

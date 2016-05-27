#include "stdafx.h"
#include "Cursor.h"


Cursor::Cursor(Game* game, IPlayerInput* playerInput) : game(game), playerInput(playerInput)
{
	//cursorModel.setFillColor(sf::Color::White);
	//cursorModel.setOrigin(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
	cursorModel.setTexture(this->game->texmgr.getRef("cursor"));
	cursorModel.setOrigin(16,16);
}


Cursor::~Cursor()
{
}

sf::Vector2f Cursor::getPosition() const
{
	return cursorModel.getPosition();
}

void Cursor::setPosition(sf::Vector2f pos)
{
	cursorModel.setPosition(pos);
}

void Cursor::draw(sf::RenderWindow& window)
{
	window.draw(cursorModel);
}

void Cursor::update(float dt)
{
	sf::Vector2f mousePos = sf::Vector2f(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window)));
	setPosition(mousePos);
	cursorModel.setRotation(playerInput->getAngle());
}
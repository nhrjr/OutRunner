#pragma once
#include "IAtomicEntity.h"
#include "Game.h"
#include "IPlayerInput.h"
class Cursor :
	public IAtomicEntity
{
public:
	Cursor(Game* game, IPlayerInput* playerInput);
	~Cursor();

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;

	void update(float dt);

private:

	Game* game;

	IPlayerInput* playerInput;

	sf::Sprite cursorModel;
};


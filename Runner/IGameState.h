#pragma once
#include "Game.h"

class Game;

enum class ActionState { NONE, PANNING, SETTINGS };

class IGameState {
public:
	virtual ~IGameState() {};

	Game* game;

	virtual void draw(float) = 0;
	virtual void update(float) = 0;
	virtual void handleInput() = 0;
	virtual bool end() = 0;

};

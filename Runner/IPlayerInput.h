#pragma once
#include <SFML\Graphics.hpp>
class IPlayerInput {
public:
	virtual float getAngle() = 0;
	virtual sf::Vector2f getOffset() = 0;
	virtual int getAction() = 0;
	virtual int getAlternateAction() = 0;
};

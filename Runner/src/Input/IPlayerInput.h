#pragma once
#include <SFML/Graphics.hpp>
class IPlayerInput {
public:
	virtual void getInput(float dt) {};
	virtual void getInput(float dt, const std::stack<NetworkPlayerEvent>& events) {};
	virtual float getAngle() = 0;
	virtual sf::Vector2f getPosition(sf::Vector2f lastPos) = 0;
	virtual int getAction() = 0;
	virtual int getAlternateAction() = 0;
	virtual void handleInput() = 0;
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual std::vector<int> getEvent() = 0;
};

#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "GuiBasicElement.h"

class GuiHealthbar :
	public GuiBasicElement
{
public:
	//GuiHealthbar();
	GuiHealthbar(std::string message, const sf::Vector2f& size);
	GuiHealthbar(const sf::Vector2f& size);
	~GuiHealthbar();

	sf::RectangleShape healthBarFull;
	sf::RectangleShape healthBarEmpty;
	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//void setPosition(const sf::Vector2f& pos);
	//void setSize(const sf::Vector2f& size);
	void setValue(const float& per);
	//void move(const sf::Vector2f& vec);
	//void setOrigin(const sf::Vector2f& origin);
	//sf::Vector2f getPosition();
	//sf::Vector2f getOrigin();
	virtual void changeElement(std::string element, float message);

};


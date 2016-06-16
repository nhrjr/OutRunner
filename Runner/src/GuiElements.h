#pragma once
#include <SFML\Graphics.hpp>
#include "GuiBasicElement.h"

#include <memory>



//class GuiListEntry
//{
//public:
//	GuiListEntry(const std::string& message, sf::RectangleShape shape, sf::Text text)
//		: message(message), shape(shape), text(text) {};
//
//	sf::RectangleShape shape;
//	std::string message;
//	sf::Text text;
//
//};

class GuiElement : public sf::Transformable, public sf::Drawable
	
{
public:
	GuiElement(std::vector<std::shared_ptr<GuiBasicElement>> e) : visible(false) {
		this->basicElements.insert(basicElements.end(), e.begin(), e.end());
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::vector<std::shared_ptr<GuiBasicElement>> basicElements;

	void show();
	void hide();
	bool visible;
	
	void highlight(const int entry);

	std::string activate(const int entry);
	std::string activate(const sf::Vector2f mousePos);

	int GuiElement::getBasicElement(const sf::Vector2f mousePos);
	int GuiElement::getBasicElement(const std::string message);

	//void setDimensions(sf::Vector2f dimensions);
	//
	sf::Vector2f dimensions;
};
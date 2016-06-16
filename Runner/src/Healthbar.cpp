#include "stdafx.h"
#include "Healthbar.h"
#include "GameSettingConstants.h"

//GuiHealthbar::GuiHealthbar()
//{
//
//}

GuiHealthbar::GuiHealthbar(std::string message, sf::Vector2f& size) : GuiBasicElement(message,size)
{
	healthBarFull.setSize(size);
	healthBarFull.setOutlineColor(sf::Color::Black);
	healthBarFull.setFillColor(sf::Color::Green);
	healthBarFull.setOutlineThickness(-2.0f);

	healthBarEmpty.setSize(size);
	healthBarEmpty.setOutlineColor(sf::Color::Black);
	healthBarEmpty.setFillColor(sf::Color::Red);
	healthBarEmpty.setOutlineThickness(-2.0f);
}

GuiHealthbar::GuiHealthbar(sf::Vector2f& size) : GuiBasicElement("healthbar", size)
{
	healthBarFull.setSize(size);
	healthBarFull.setOutlineColor(sf::Color::Black);
	healthBarFull.setFillColor(sf::Color::Green);
	healthBarFull.setOutlineThickness(-2.0f);

	healthBarEmpty.setSize(size);
	healthBarEmpty.setOutlineColor(sf::Color::Black);
	healthBarEmpty.setFillColor(sf::Color::Red);
	healthBarEmpty.setOutlineThickness(-2.0f);
}


GuiHealthbar::~GuiHealthbar()
{
}

void GuiHealthbar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!this->visible) return;
	states.transform *= getTransform();
	target.draw(healthBarEmpty,states);
	target.draw(healthBarFull,states);
}

//void GuiHealthbar::setPosition(const sf::Vector2f& pos) {
//	healthBarEmpty.setPosition(pos);
//	healthBarFull.setPosition(pos);
//	
//}

//void GuiHealthbar::setSize(const sf::Vector2f& size) {
//	healthBarEmpty.setSize(size);
//	healthBarFull.setSize(size);
//}

void GuiHealthbar::setValue(const float& per) {
	sf::Vector2f size = this->dimensions;
	this->healthBarFull.setSize(sf::Vector2f(size.x * per, size.y));
}

void GuiHealthbar::changeElement(std::string element, float message)
{
	if (element == "hitpoints")
	{
		setValue(message * 0.01f);
	}
}

//void GuiHealthbar::move(const sf::Vector2f& vec) {
//	healthBarEmpty.move(vec);
//	healthBarFull.move(vec);
//}

//void GuiHealthbar::setOrigin(const sf::Vector2f& origin) {
//	healthBarEmpty.setOrigin(origin);
//	healthBarFull.setOrigin(origin);
//}

//sf::Vector2f GuiHealthbar::getPosition() {
//	return healthBarEmpty.getPosition();
//}

//sf::Vector2f GuiHealthbar::getOrigin() {
//	return healthBarEmpty.getOrigin();
//}

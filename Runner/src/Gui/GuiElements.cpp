#include "stdafx.h"
#include "GuiElements.h"
#include "Core/Console.h"

void GuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!this->visible) return;

	states.transform *= getTransform();
	for (auto entry : this->basicElements)
	{
		target.draw(*entry, states);
	}
}

void GuiElement::hide() {
	this->visible = false;
	for (auto& basic : basicElements)
	{
		basic->hide();
	}
}

void GuiElement::show()
{
	this->visible = true;
	for (auto& basic : basicElements)
	{
		basic->show();
	}
}

void GuiElement::highlight(const int entry)
{
	for (int i = 0; i < this->basicElements.size(); ++i)
	{
		if (i == entry)
		{
			basicElements[i]->highlight(true);
		}
		else
		{
			basicElements[i]->highlight(false);
		}
	}
}

std::string GuiElement::activate(const int entry)
{
	if (entry == -1) return "null";
	return this->basicElements[entry]->message;
}

std::string GuiElement::activate(sf::Vector2f mousePos)
{
	int entry = this->getBasicElement(mousePos);
	return this->activate(entry);
}

int GuiElement::getBasicElement(const sf::Vector2f mousePos)
{
	if (basicElements.size() == 0) return -1;
	if (!this->visible) return -1;

	for (int i = 0; i < this->basicElements.size(); ++i)
	{
		sf::Vector2f point = basicElements[i]->getInverseTransform().transformPoint(getInverseTransform().transformPoint(mousePos));

		if (point.x < 0 || point.x > this->basicElements[i]->getScale().x * this->basicElements[i]->dimensions.x) continue;
		if (point.y < 0 || point.y > this->basicElements[i]->getScale().y * this->basicElements[i]->dimensions.y) continue;
		return i;
	}
	return -1;
}

int GuiElement::getBasicElement(const std::string message)
{
	if (basicElements.size() == 0) return -1;
	if (!this->visible) return -1;

	for (int i = 0; i < this->basicElements.size(); ++i)
	{
		if (basicElements[i]->message == message)
			return i;
	}
	return -1;
}

//void GuiElement::setDimensions(sf::Vector2f dimensions)
//{
//	this->dimensions = dimensions;
//
//	//for (auto& entry : entries)
//	//{
//	//	entry.shape.setSize(dimensions);
//	//	entry.text.setCharacterSize(dimensions.y - 2 * style.borderSize - 2 * padding);
//	//}
//}
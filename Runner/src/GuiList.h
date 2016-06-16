#pragma once
#include "GuiElements.h"
#include "Console.h"

class GuiList : public GuiElement
{
public:
	GuiList(std::vector<std::shared_ptr<GuiBasicElement>> e, bool horizontal )
		: GuiElement(e), horizontal(horizontal)
	{
		sf::Vector2f offset = sf::Vector2f(0,0);
		for (auto& entry : this->basicElements)
		{
			sf::Vector2f origin = this->getOrigin();
			origin -= offset;

			entry->setOrigin(origin);
			entry->setPosition(this->getPosition());
			
			if (this->horizontal) offset.x += entry->dimensions.x;
			else offset.y += entry->dimensions.y;
		}
		this->dimensions = offset;
		
	};

private:
	bool horizontal;
};
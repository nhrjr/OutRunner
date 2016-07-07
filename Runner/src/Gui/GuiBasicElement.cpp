#include "stdafx.h"
#include "GuiBasicElement.h"
#include "Core/Console.h"


void GuiBasicElement::hide() {
	this->visible = false;
}

void GuiBasicElement::show()
{
	this->visible = true;
}

void GuiBasicElement::highlight(bool doit)
{

}

void GuiBasicElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}

void GuiBasicElement::changeElement(std::string element, std::string message){}
void GuiBasicElement::changeElement(std::string element, float messaage){}

void GuiButtonText::highlight(bool doit)
{
	if(doit)
	{
		this->shape.setFillColor(style.bodyHighlightColor);
		this->shape.setOutlineColor(style.borderHighlightColor);
		this->text.setColor(style.textHighlightColor);
	}
	else
	{
		this->shape.setFillColor(style.bodyColor);
		this->shape.setOutlineColor(style.borderColor);
		this->text.setColor(style.textColor);
	}
}

void GuiButtonText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!this->visible) return;
	states.transform *= getTransform();
	target.draw(shape,states);
	target.draw(text, states);
}

void GuiButtonText::changeElement(std::string element, std::string message)
{
	if (element == "text")
	{
		text.setString(message);
	}
}


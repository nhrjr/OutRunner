#pragma once

#include <vector>
#include <utility>
#include <string>
#include <iostream>

class GuiStyle 
{
public:
	GuiStyle() {};
	~GuiStyle() {};
	GuiStyle(sf::Font* font, float borderSize, sf::Color bodyColor, sf::Color borderColor, sf::Color textColor,
		sf::Color bodyHighlightColor, sf::Color borderHighlightColor, sf::Color textHighlightColor)
		/*: font(font), borderSize(borderSize),
		bodyColor(bodyColor), bodyHighlightColor(bodyHighlightColor),
		textColor(textColor), textHighlightColor(textHighlightColor),
		borderColor(borderColor), borderHighlightColor(borderHighlightColor) */
	{
		this->font = font;
		this->borderSize = borderSize;
		this->bodyColor=bodyColor;
		this->bodyHighlightColor=bodyHighlightColor;
		this->textColor=textColor;
		this->textHighlightColor=textHighlightColor;
		this->borderColor=borderColor;
		this->borderHighlightColor=borderHighlightColor;
	};

	sf::Font* font;
	sf::Color bodyColor;
	sf::Color bodyHighlightColor;
	sf::Color borderColor;
	sf::Color borderHighlightColor;
	sf::Color textColor;
	sf::Color textHighlightColor;

	float borderSize;
};

class GuiEntry
{
public:
	GuiEntry(const std::string& message, sf::RectangleShape shape, sf::Text text)
		//: message(message), shape(shape), text(text)
	{
		this->shape = shape;
		this->message = message;
		this->text = text;
	};

	sf::RectangleShape shape;
	std::string message;
	sf::Text text;

};

class GUI : public sf::Transformable, public sf::Drawable
{
public:
	GUI(sf::Vector2f dimensions, int padding, bool horizontal, GuiStyle& style, std::vector<std::pair<std::string, std::string>> entries)
	//: visible(false), horizontal(horizontal), style(style), dimensions(dimensions), padding(padding)
	{
		this->visible = false;
		this->horizontal = horizontal;
		this->style = style;
		this->dimensions = dimensions;
		this->padding = padding;

		sf::RectangleShape shape;
		shape.setSize(dimensions);
		shape.setFillColor(style.bodyColor);
		shape.setOutlineThickness(-style.borderSize);
		shape.setOutlineColor(style.borderColor);

		for (auto entry : entries)
		{
			sf::Text text;
			text.setString(entry.first);
			text.setFont(*style.font);
			text.setColor(style.textColor);
			text.setCharacterSize(unsigned int (dimensions.y - 2 *style.borderSize - 2* padding));

			this->entries.push_back(GuiEntry(entry.second, shape, text));
		}
	};

	~GUI();

	sf::Vector2f getSize();

	int getEntry(const sf::Vector2f mousePos);
	int getEntry(const std::string message);

	void setEntryText(int entry, std::string text);

	void setDimensions(sf::Vector2f dimensions);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void show();
	void hide();

	void highlight(const int entry);

	std::string activate(const int entry);
	std::string activate(const sf::Vector2f mousePos);

	std::vector<GuiEntry> entries;
	bool visible;

private:
	bool horizontal;

	GuiStyle style;

	sf::Vector2f dimensions;

	int padding;

};


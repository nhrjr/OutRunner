#pragma once
#include <SFML/Graphics.hpp>

#include <memory>

class ButtonStyle
{
public:
	ButtonStyle() {};
	~ButtonStyle() {};
	ButtonStyle(sf::Font* font, float borderSize, sf::Color bodyColor, sf::Color borderColor, sf::Color textColor,
		sf::Color bodyHighlightColor, sf::Color borderHighlightColor, sf::Color textHighlightColor)
		: font(font), borderSize(borderSize),
		bodyColor(bodyColor), bodyHighlightColor(bodyHighlightColor),
		textColor(textColor), textHighlightColor(textHighlightColor),
		borderColor(borderColor), borderHighlightColor(borderHighlightColor) {};

	sf::Font* font;
	sf::Color bodyColor;
	sf::Color bodyHighlightColor;
	sf::Color borderColor;
	sf::Color borderHighlightColor;
	sf::Color textColor;
	sf::Color textHighlightColor;

	float borderSize;
};

class GuiBasicElement : public sf::Transformable, public sf::Drawable
{
public:
	GuiBasicElement(std::string message, sf::Vector2f d) : dimensions(d), message(message), visible(false) {}
	//GuiBasicElement() = delete;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void highlight(bool doit);

	virtual void changeElement(std::string, std::string);
	virtual void changeElement(std::string, float);
	
	//std::string activate(const sf::Vector2f mousePos);

	std::string message;
	void show();
	void hide();
	bool visible;
	sf::Vector2f dimensions;
};

class GuiButtonText : public GuiBasicElement
{
public:
	GuiButtonText(std::string t, sf::Vector2f d, int padding, ButtonStyle& style, std::string message) : GuiBasicElement(message, d), style(style), text(t,*style.font, static_cast<unsigned int>(d.y) - 2 * padding), shape(d) {
		text.setFillColor(style.textColor);
		
		shape.setFillColor(style.bodyColor);
		shape.setOutlineColor(style.borderColor);
		shape.setOutlineThickness(style.borderSize);
		text.move(sf::Vector2f(static_cast<float>(padding), 0.0f));
	}
	~GuiButtonText() = default;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void highlight(bool doit);

	virtual void changeElement(std::string element, std::string message);
	ButtonStyle style;
	sf::Text text;
	sf::RectangleShape shape;
};

//class GuiText : public GuiBasicElement
//{
//
//};
#include "stdafx.h"
#include "MainMenu.h"



MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
}

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window)
{
	// Load menu image from file
	sf::Texture texture;
	texture.loadFromFile("images/mainmenu.png");
	sf::Sprite sprite(texture);

	//Setup clickable regions

	//Play Menu item coordinates
	MenuItem playButton;
	playButton.rect.top = 145;
	playButton.rect.left = 0;
	playButton.rect.height = 235;
	playButton.rect.width = 1024;
	playButton.action = Play;

	// Exit Menu item coordinates
	MenuItem exitButton;
	exitButton.rect.top = 383;
	exitButton.rect.left = 0;
	exitButton.rect.height = 177;
	exitButton.rect.width = 1024;
	exitButton.action = Exit;

	_menuItems.push_back(playButton);
	_menuItems.push_back(exitButton);

	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
	for (auto&  it : _menuItems) {
		sf::Rect<int> menuItemRect = it.rect;
		if (menuItemRect.contains(x,y)) {
			return it.action;
		}
	}

	return Nothing;
}

MainMenu::MenuResult MainMenu::GetMenuResponse(sf::RenderWindow& window) {
	sf::Event menuEvent;
	while (true) {
		while (window.pollEvent(menuEvent)) {
			if (menuEvent.type == sf::Event::MouseButtonPressed) {
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			}
			if (menuEvent.type == sf::Event::Closed) {
				return Exit;
			}
		}
	}
}

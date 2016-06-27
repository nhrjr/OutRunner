#include "stdafx.h"
#include "GUI.h"
#include "GameSettingConstants.h"
#include "Healthbar.h"

GuiPlaying::GuiPlaying()
{

}

GuiPlaying::GuiPlaying(Game* game)
{
	this->game = game;

	sf::Vector2f dim(64, 32);
	int padding = 2;
	std::vector<std::shared_ptr<GuiBasicElement>> F10 = {
		std::make_shared<GuiButtonText>("F10",dim,2, game->styleSheets.at("button_text"),"F10")
	};

	dim = sf::Vector2f(GAME_MENU_BUTTON_WIDTH * 2, GAME_MENU_BUTTON_HEIGHT);
	std::vector<std::shared_ptr<GuiBasicElement>> settingsEntries = {
		std::make_shared<GuiButtonText>("Resume",dim,padding, game->styleSheets.at("button_text"),"resume"),
		std::make_shared<GuiButtonText>("Quit to Menu",dim,padding, game->styleSheets.at("button_text"), "quit_to_menu"),
		std::make_shared<GuiButtonText>("Exit Game",dim,padding, game->styleSheets.at("button_text"),"exit_game")
	};

	//dim = sf::Vector2f(64, 32);
	std::vector<std::shared_ptr<GuiBasicElement>> playerEntries = {
		std::make_shared<GuiHealthbar>("hitpoints",sf::Vector2f(100,32)),
		std::make_shared<GuiButtonText>("Weapon",sf::Vector2f(150,28),4, game->styleSheets.at("text"),"weapon"),
		std::make_shared<GuiButtonText>("Ammo", sf::Vector2f(100,28),4,game->styleSheets.at("text"),"ammo")
	};

	this->guiElements.emplace("F10", std::make_shared<GuiList>(F10, true));
	this->guiElements.at("F10")->setPosition(sf::Vector2f(static_cast<float>(this->game->window.getSize().x) - 64.f, 0.f));
	this->guiElements.at("F10")->show();

	this->guiElements.emplace("settings", std::make_shared<GuiList>(settingsEntries, false));
	this->guiElements.at("settings")->setOrigin(GAME_MENU_BUTTON_WIDTH , GAME_MENU_BUTTON_HEIGHT / 2);
	this->guiElements.at("settings")->setPosition(sf::Vector2f(this->game->window.getSize().x * 0.5f, this->game->window.getSize().y * 0.5f));

	this->guiElements.emplace("playerbox", std::make_shared<GuiList>( playerEntries, true));
	this->guiElements.at("playerbox")->setOrigin(50, 16);
	this->guiElements.at("playerbox")->setPosition(sf::Vector2f(static_cast<float>(this->game->window.getSize().x ) * 0.5f, static_cast<float>(this->game->window.getSize().y) - 16.f));
	this->guiElements.at("playerbox")->show();
}

void GuiPlaying::resize()
{
	this->guiElements.at("F10")->setPosition(sf::Vector2f(static_cast<float>(this->game->window.getSize().x) - 64.f, 0));
	this->guiElements.at("F10")->show();
	this->guiElements.at("settings")->setPosition(sf::Vector2f(static_cast<float>(this->game->window.getSize().x) * 0.5f, static_cast<float>(this->game->window.getSize().y) * 0.5f));
	this->guiElements.at("settings")->hide();
	this->guiElements.at("playerbox")->setPosition(sf::Vector2f(static_cast<float>(this->game->window.getSize().x) * 0.5f, static_cast<float>(this->game->window.getSize().y) - 16.f));
	this->guiElements.at("playerbox")->show();
}

void GuiPlaying::highlight(sf::Vector2f mouse) 
{
	this->guiElements.at("settings")->highlight(this->guiElements.at("settings")->getBasicElement(mouse));
	this->guiElements.at("F10")->highlight(this->guiElements.at("F10")->getBasicElement(mouse));
}

void GuiPlaying::draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	for (auto& gui : guiElements) target.draw(*gui.second);
}

std::string GuiPlaying::activate(sf::Vector2f mousePos)
{
	for (auto& element : guiElements)
	{
		int entry = element.second->getBasicElement(mousePos);
		if(entry >= 0) return element.second->activate(entry);
	}
	return "null";
}

void GuiPlaying::setPlayerHitpoints(float hitpoints)
{
	int entry = this->guiElements.at("playerbox")->getBasicElement("hitpoints");
	this->guiElements.at("playerbox")->basicElements[entry]->changeElement("hitpoints", hitpoints);
}

void GuiPlaying::setPlayerWeapon(std::string name)
{
	int entry = this->guiElements.at("playerbox")->getBasicElement("weapon");
	this->guiElements.at("playerbox")->basicElements[entry]->changeElement("text", name);
}

void GuiPlaying::setPlayerAmmo(int ammo)
{
	int entry = this->guiElements.at("playerbox")->getBasicElement("ammo");
	this->guiElements.at("playerbox")->basicElements[entry]->changeElement("text", std::to_string(ammo));
}
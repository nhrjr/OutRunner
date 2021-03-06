#pragma once

#include "Core/Game.h"
#include "Gui/GuiElements.h"
#include "Gui/GuiFrame.h"
#include "Gui/GuiList.h"

#include "Algorithms/signal_slot.h"

#include <memory>


class GuiPlaying : public sf::Drawable, public Nano::Observer
{
public:
	GuiPlaying();
	GuiPlaying(Game* game);

	Game* game;

	void resize();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void highlight(sf::Vector2f mouse);
	std::string activate(sf::Vector2f mousePos);

	std::unordered_map<std::string, std::shared_ptr<GuiElement>> guiElements;

	void setPlayerHitpoints(float hitpoints);
	void setPlayerWeapon(std::string name);
	void setPlayerAmmo(int ammo);

};
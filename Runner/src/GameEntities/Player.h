#pragma once
#include "GameEntities/IGameEntity.h"
#include "Core/GameSettingConstants.h"
#include "Core/Game.h"
#include "Input/MKInput.h"
#include "Input/NetworkInput.h"
#include "Weapons/Weapon.h"
#include "Gui/Healthbar.h"
#include "GameEntities/AnimatedModel.h"

#include "Algorithms/signal_slot.h"

class Player :
	public IGameEntity, public Nano::Observer
{
	Game* game;
	IPlayerInput* playerInput;

	AnimatedModel playerModel;
	
	GuiHealthbar healthbar;

	float healingTimer;
	bool remote;

public:
	Player(Game* game, IPlayerInput* playerInput, bool r);
	Player(Game* game, IPlayerInput* playerInput,const Guid& guid);
	~Player();

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;
	virtual void update(float dt);
	void collide(IGameEntity& other, unsigned int type, float dt);

	std::vector<std::shared_ptr<Weapon>> weapons;

	std::vector<std::shared_ptr<Weapon>>::const_iterator weapon;

	void nextWeapon();
	void prevWeapon();

	bool isDead = false;
	float hitboxRadius;
	float attackingAngle;
	//Signals
	Nano::Signal<void(float)> changedHitpoints;
	Nano::Signal<void(std::string)> changedWeapon;
	Nano::Signal<void(int)> changedAmmo;

	sf::CircleShape facingDot;
	sf::CircleShape walkingDirectionDot;

private:
	virtual sf::Vector2f getPoint(int i) const;
	virtual unsigned int getPointCount() const;
	virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


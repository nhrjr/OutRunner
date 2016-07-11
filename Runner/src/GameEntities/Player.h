#pragma once
#include "GameEntities/IAtomicEntity.h"
#include "Core/GameSettingConstants.h"
#include "Core/Game.h"
#include "Input/MKInput.h"
#include "Input/NetworkInput.h"
#include "Weapons/Weapon.h"
#include "Gui/Healthbar.h"
#include "GameEntities/AnimatedModel.h"

#include "Algorithms/signal_slot.h"

class Player :
	public IAtomicEntity, public Nano::Observer
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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void update(float dt);
	void collide(IAtomicEntity& other, unsigned int type, float dt);

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
	virtual float getMinDistance() const;
};


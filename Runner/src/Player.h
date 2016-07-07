#pragma once
#include "IAtomicEntity.h"
#include "GameSettingConstants.h"
#include "Game.h"
#include "MKInput.h"
#include "NetworkInput.h"
#include "Weapon.h"
#include "Healthbar.h"
#include "AnimatedModel.h"

#include "signal_slot.h"

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

	//virtual void draw(sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//virtual void setPosition(sf::Vector2f pos);
	//virtual sf::Vector2f getPosition() const;
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
	//virtual sf::Vector2f getPoint(int i) const;
	//virtual unsigned int getPointCount() const;
	//virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


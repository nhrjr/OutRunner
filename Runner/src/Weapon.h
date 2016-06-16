#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"
#include "Projectile.h"
#include "signal_slot.h"
#include "AnimatedModel.h"
#include "StateMachine.h"

#include "Console.h"

namespace WEAPON
{
	enum WEAPON {
		IDLE,
		SHOOT
	};
}

class Weapon :
	public IGameEntity, public StateMachine
{
protected:
	Weapon(std::string name, TextureManager& t);
public:
	
	~Weapon();

	float cooldownTime;
	float reloadTime;

	unsigned int ammo;
	unsigned int ammoCap;

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;
	virtual sf::Vector2f getBarrelPosition() const;

	virtual void update(float dt);
	void attachedMove(sf::Vector2f directionOffset, float angleOffset);
	void attachedSetPosition(sf::Vector2f newPos, float angleOffset);
	
	virtual void shoot() = 0;

	AnimatedModel weaponModel;

	std::string name;

	std::vector<std::shared_ptr<Projectile>> spawnedEntities;

private:
	
};



class WeaponState : public BaseState {
public:
	WeaponState(Weapon* w) : weapon(w) {};
	Weapon* weapon;
	bool stateLocked = false;
};

class ReadyToFire;
class Discharge;
class Cooldown;
class Reload;

class Cooldown : public WeaponState {
	float cooldownTimer;
	
public:
	Cooldown(Weapon* weapon) : WeaponState(weapon), cooldownTimer(0.0f) {}

	virtual void enterState(BaseState* state)
	{
		//weapon->ready = false;
		stateLocked = true;
	}
	virtual void updateState(float dt)
	{
		cooldownTimer += dt;
		if (cooldownTimer >= weapon->cooldownTime)
		{
			stateLocked = false;
			if (weapon->ammo == 0)
			{
				_stateMachine->enterState<Reload>();
				return;
			}
			_stateMachine->enterState<ReadyToFire>();
		}
	}
	virtual void exitState(BaseState* state)
	{
		//weapon->ready = true;

		weapon->weaponModel.spriteDefs[0].setnextState(WEAPON::IDLE);
		cooldownTimer = 0.0f;
	}
	virtual bool isValidState(BaseState* state)
	{
		if (stateLocked) return false;
		if (typeid(*state) == typeid(Reload)
		 || typeid(*state) == typeid(ReadyToFire))
			return true;
		return false;
	}

};

class Reload : public WeaponState {
	float reloadTimer;
	
public:
	Reload(Weapon* weapon) : WeaponState(weapon), reloadTimer(0.0f) {}
	virtual void enterState(BaseState* state)
	{
		//weapon->ready = false;
		stateLocked = true;
	}
	virtual void exitState(BaseState* state)
	{
		//weapon->ready = true;
		reloadTimer = 0.0f;
	}
	virtual void updateState(float dt)
	{
		reloadTimer += dt;
		if (reloadTimer >= weapon->reloadTime)
		{
			stateLocked = false;
			weapon->ammo = weapon->ammoCap;
			_stateMachine->enterState<ReadyToFire>();
		}
	}
	virtual bool isValidState(BaseState* state)
	{
		if (stateLocked) return false;
		if (typeid(*state) == typeid(ReadyToFire))
			return true;
		return false;
	}
};

class Discharge : public WeaponState {
	
public:
	Discharge(Weapon* weapon) : WeaponState(weapon) {}
	virtual void enterState(BaseState* state)
	{
		//weapon->triggered = true;
		weapon->shoot();
		--weapon->ammo;
		weapon->weaponModel.spriteDefs[0].setnextState(WEAPON::SHOOT);
	}
	virtual void updateState(float dt)
	{
		_stateMachine->enterState<Cooldown>();
	}
	//virtual void exitState(BaseState* state)
	//{
	//	//weapon->triggered = false;
	//}
	virtual bool isValidState(BaseState* state)
	{
		if (typeid(*state) == typeid(Cooldown))
			return true;
		return false;
	}
};

class ReadyToFire : public WeaponState {
	
public:
	ReadyToFire(Weapon* weapon) : WeaponState(weapon) {}

	virtual bool isValidState(BaseState* state)
	{
		if (typeid(*state) == typeid(Discharge)
			|| typeid(*state) == typeid(Reload))
			return true;
		return false;
	}
};




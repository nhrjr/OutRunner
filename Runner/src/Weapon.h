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

class Cooldown : public WeaponState {
	float cooldownTimer;
	
public:
	Cooldown(Weapon* weapon);

	virtual void enterState(BaseState* state);

	virtual void updateState(float dt);

	virtual void exitState(BaseState* state);

	virtual bool isValidNextState(BaseState* state);



};

class Reload : public WeaponState {
	float reloadTimer;
	
public:
	Reload(Weapon* weapon);
	virtual void enterState(BaseState* state);

	virtual void exitState(BaseState* state);

	virtual void updateState(float dt);

	virtual bool isValidNextState(BaseState* state);

};

class Discharge : public WeaponState {
	
public:
	Discharge(Weapon* weapon) ;
	virtual void enterState(BaseState* state);

	virtual void exitState(BaseState* state) {};

	virtual void updateState(float dt);

	virtual bool isValidNextState(BaseState* state);

};

class ReadyToFire : public WeaponState {
	
public:
	ReadyToFire(Weapon* weapon);

	virtual void enterState(BaseState* state) {};

	virtual void exitState(BaseState* state) {};

	virtual void updateState(float dt) {};

	virtual bool isValidNextState(BaseState* state);
};




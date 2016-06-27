#include "stdafx.h"
#include "Weapon.h"
#include "Console.h"

#include <cmath>


struct {
	Animation w_idle{ 0, 0, 0.0f , 0 , 0, false, -1 };
	Animation w_shoot{ 0, 19, 0.01f, 192, 108, false, 0 };

} animWeapon;

std::unordered_map<int, Animation> animationsWeapon
{
	std::make_pair(WEAPON::IDLE, animWeapon.w_idle),
	std::make_pair(WEAPON::SHOOT, animWeapon.w_shoot)
};

Weapon::Weapon(std::string n, TextureManager& t) : name(n)
	,weaponModel(SpriteDefinition(animationsWeapon, sf::Vector2f(50.f, 54.f), 0.1f, t.getRef(n)))
{
	weaponModel.spriteDefs[0].setnextState(WEAPON::IDLE);

	this->addState<Cooldown>(this);
	this->addState<Reload>(this);
	this->addState<ReadyToFire>(this);
	this->addState<Discharge>(this);
	this->enterState<ReadyToFire>();
}

Weapon::~Weapon()
{
}

void Weapon::setPosition(sf::Vector2f pos)
{
	weaponModel.setPosition(pos);
}

sf::Vector2f Weapon::getPosition() const
{
	return weaponModel.getPosition();
	return sf::Vector2f(0, 0);
}

sf::Vector2f Weapon::getBarrelPosition() const
{
	sf::Vector2f tmp = weaponModel.getOrigin();
	float angle = weaponModel.getRotation() * M_PI / 180.0f;
	float cangle = cos(angle);
	float sangle = sin(angle);
	tmp = getPosition() - sf::Vector2f(tmp.x * cangle - tmp.y * sangle, tmp.y * cangle +tmp. x * sangle);
	return tmp;
}

void Weapon::draw(sf::RenderWindow& window)
{
	//window.draw(weaponModel);
}

void Weapon::attachedMove(sf::Vector2f directionOffset, float angleOffset)
{
	this->weaponModel.setRotation(angleOffset);
	this->weaponModel.move(directionOffset);
}

void Weapon::attachedSetPosition(sf::Vector2f newPos, float angleOffset)
{
	this->weaponModel.setRotation(angleOffset);
	this->weaponModel.setPosition(newPos);
}

void Weapon::update(float dt)
{
	spawnedEntities.clear();
	this->updateWithDeltaTime(dt);

	weaponModel.update(dt);
}

Cooldown::Cooldown(Weapon* weapon) : WeaponState(weapon), cooldownTimer(0.0f) {}
void Cooldown::enterState(BaseState* state)
{
	stateLocked = true;
}
void Cooldown::updateState(float dt)
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
void Cooldown::exitState(BaseState* state)
{
	weapon->weaponModel.spriteDefs[0].setnextState(WEAPON::IDLE);
	cooldownTimer = 0.0f;
}
bool Cooldown::isValidNextState(BaseState* state)
{
	if (stateLocked) return false;
	if (typeid(*state) == typeid(Reload)
	 || typeid(*state) == typeid(ReadyToFire))
		return true;
	return false;
}

Reload::Reload(Weapon* weapon) : WeaponState(weapon), reloadTimer(0.0f) {}
void Reload::enterState(BaseState* state)
{
	stateLocked = true;
}
void Reload::exitState(BaseState* state)
{
	reloadTimer = 0.0f;
}
void Reload::updateState(float dt)
{
	reloadTimer += dt;
	if (reloadTimer >= weapon->reloadTime)
	{
		stateLocked = false;
		weapon->ammo = weapon->ammoCap;
		_stateMachine->enterState<ReadyToFire>();
	}
}
bool Reload::isValidNextState(BaseState* state)
{
	if (stateLocked) return false;
	if (typeid(*state) == typeid(ReadyToFire))
		return true;
	return false;
}

Discharge::Discharge(Weapon* weapon) : WeaponState(weapon) {}
void Discharge::enterState(BaseState* state)
{
	weapon->shoot();
	--weapon->ammo;
	weapon->weaponModel.spriteDefs[0].setnextState(WEAPON::SHOOT);
}
void Discharge::updateState(float dt)
{
	_stateMachine->enterState<Cooldown>();
}

bool Discharge::isValidNextState(BaseState* state)
{
	if (typeid(*state) == typeid(Cooldown))
		return true;
	return false;
}

ReadyToFire::ReadyToFire(Weapon* weapon) : WeaponState(weapon) {}
bool ReadyToFire::isValidNextState(BaseState* state)
{
	if (typeid(*state) == typeid(Discharge)
		|| typeid(*state) == typeid(Reload))
		return true;
	return false;
}

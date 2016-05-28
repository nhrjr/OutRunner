#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"
#include "Game.h"
#include "MKInput.h"
#include "NetworkInput.h"
#include "Weapon.h"


class Player :
	public IGameEntity
{
	Game* game;
	IPlayerInput* playerInput;

	sf::CircleShape playerModel;
	sf::CircleShape playerHead;
	sf::RectangleShape healthBarFull;
	sf::RectangleShape healthBarEmpty;

	float healingTimer;
	bool remote;

public:
	Player();
	Player(Game* game, IPlayerInput* playerInput);
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

private:
	virtual sf::Vector2f getPoint(int i) const;
	virtual int getPointCount() const;
	virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


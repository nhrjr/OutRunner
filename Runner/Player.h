#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"
#include "Game.h"
#include "MKInput.h"
#include "Weapon.h"
#include "Bullet.h"

class Player :
	public IGameEntity
{
	Game* game;
	IPlayerInput* playerInput;


	sf::CircleShape playerModel;
	sf::CircleShape playerHead;
	sf::RectangleShape healthBarFull;
	sf::RectangleShape healthBarEmpty;

	Weapon weapon;

	float healingTimer;

public:
	Player(Game* game, IPlayerInput* playerInput);
	~Player();
	
	virtual void draw(sf::RenderWindow& window, float dt);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;
	virtual void update(float dt);
	void collide(IGameEntity& other, unsigned int type, float dt);

	//void updateCollision(sf::Vector2f displace, float dt);

	bool isAttacking = false;
	bool isShooting = false;
	float hitboxRadius;
	float attackingAngle;
private:



	virtual sf::Vector2f getPoint(int i) const;
	virtual int getPointCount() const;
	virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
	

};


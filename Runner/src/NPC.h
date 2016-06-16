#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"
#include "Game.h"
#include "MKInput.h"
#include "SMG.h"
#include "Projectile.h"
#include "Healthbar.h"

class NPC :
	public IGameEntity
{
public:
	explicit NPC(Game* game, sf::Vector2f pos);
	~NPC();

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;
	virtual void update(float dt);

	void collide(IGameEntity& other, unsigned int type, float dt);


	void moveToTarget(std::deque<sf::Vector2f> target);
	void shootAtTarget(sf::Vector2f target);

	bool isAttacking = false;
	bool isShooting = false;
	float attackingAngle;
	float hitboxRadius;

	Game* game;

	std::deque<sf::Vector2f> targetList;
	sf::Vector2f target;

	float attackingTimer = 0.0f;

	sf::CircleShape npcModel;
	sf::CircleShape npcHead;

	GuiHealthbar healthbar;

	SMG weapon;

	virtual sf::Vector2f getPoint(int i) const;
	virtual size_t getPointCount() const;
	virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


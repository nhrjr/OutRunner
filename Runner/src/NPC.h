#pragma once
#include "IAtomicEntity.h"
#include "GameSettingConstants.h"
#include "Game.h"
#include "MKInput.h"
#include "SMG.h"
#include "Projectile.h"
#include "Healthbar.h"
#include "StateMachine.h"

class NPC :
	public IAtomicEntity, public StateMachine
{
public:
	explicit NPC(Game* game, sf::Vector2f pos);
	~NPC();

	//virtual void draw(sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//virtual void setPosition(sf::Vector2f pos);
	//virtual sf::Vector2f getPosition() const;
	virtual void update(float dt);

	void collide(IAtomicEntity& other, unsigned int type, float dt);

	sf::Vector2f direction;
	float viewAngle;

	void moveToTarget(std::deque<sf::Vector2f> target);
	void moveToTarget(sf::Vector2f target);
	void shootAtTarget(sf::Vector2f target);

	bool isAttacking = false;
	bool isShooting = false;
	float attackingAngle;
	float hitboxRadius;

	Game* game;

	std::deque<sf::Vector2f> targetList;
	sf::Vector2f target;

	sf::CircleShape npcModel;
	sf::CircleShape npcHead;

	GuiHealthbar healthbar;

	SMG weapon;

	//virtual sf::Vector2f getPoint(int i) const;
	//virtual unsigned int getPointCount() const;
	//virtual sf::Transform getTransform() const;
	virtual float getMinDistance() const;
};


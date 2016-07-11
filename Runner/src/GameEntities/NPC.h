#pragma once
#include "GameEntities/IAtomicEntity.h"
#include "Core/GameSettingConstants.h"
#include "Core/Game.h"
#include "Input/MKInput.h"
#include "Weapons/SMG.h"
#include "Projectiles/Projectile.h"
#include "Gui/Healthbar.h"
#include "Algorithms/StateMachine.h"

class NPC :
	public IAtomicEntity, public StateMachine
{
public:
	explicit NPC(Game* game, sf::Vector2f pos);
	~NPC();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
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

	sf::CircleShape facingDot;

	GuiHealthbar healthbar;

	SMG weapon;
	
	virtual float getMinDistance() const;
};


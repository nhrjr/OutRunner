#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"
#include "Bullet.h"

class Weapon :
	public IGameEntity
{
public:
	Weapon();
	~Weapon();

	float reloadTime;
	float reloadTimer;
	bool ready;
	bool triggered;

	virtual void draw(sf::RenderWindow& window);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;

	virtual void update(float dt);
	void attachedMove(sf::Vector2f directionOffset, float angleOffset);
	void attachedSetPosition(sf::Vector2f newPos, float angleOffset);
	std::shared_ptr<Bullet> shoot();
	void trigger();

private:
	sf::RectangleShape weaponModel;
};


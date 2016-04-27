#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"

class Weapon :
	public IGameEntity
{
public:
	Weapon();
	~Weapon();

	float reloadTime;
	float reloadTimer;

	virtual void draw(sf::RenderWindow& window, float dt);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;

	virtual void update(float dt);
	void attachedMove(sf::Vector2f directionOffset, float angleOffset);

private:
	sf::RectangleShape weaponModel;
};


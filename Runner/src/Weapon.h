#pragma once
#include "IGameEntity.h"
#include "GameSettingConstants.h"
#include "Projectile.h"

//template<typename projT>
class Weapon :
	public IGameEntity
{
protected:
	Weapon();
public:
	
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
	
	void toggleTrigger();

	virtual std::vector<std::shared_ptr<Projectile>> shoot() = 0;

	sf::RectangleShape weaponModel;

private:
	
};



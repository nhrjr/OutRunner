#pragma once
#include "Game.h"
#include "IGameEntity.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"

#include "Player.h"
#include "Projectile.h"
#include "Enemies.h"

#include "guid.h"

#include <unordered_map>
#include <memory>
class GameLogicManager
{
public:
	GameLogicManager(Game* g);
	~GameLogicManager();

	Game* game;
	Map* map;
	std::shared_ptr<Player> player;

	GameObjectManager<Player> players;
	GameObjectManager<Projectile> bullets;
	GameObjectManager<Enemies> enemies;

	CollisionManager collisionManager;

	void Init(std::shared_ptr<Player> p, Map* m);

	std::deque<sf::Vector2f> testPath;
	void drawTestPath();

	void draw(sf::RenderWindow& renderWindow);
	void update(float dt);
};




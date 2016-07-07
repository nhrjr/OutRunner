#pragma once
#include "Core/Game.h"
#include "GameEntities/IGameEntity.h"
#include "Core/GameObjectManager.h"
#include "Core/CollisionManager.h"

#include "GameEntities/Player.h"
#include "Projectiles/Projectile.h"
#include "GameEntities/Enemies.h"

#include "Algorithms/guid.h"

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

	std::vector<std::shared_ptr<IGameEntity>> getVisibleObjects(std::shared_ptr<IGameEntity> ent, const GameObjectManager<IGameEntity>& toCheckAgainst);
};




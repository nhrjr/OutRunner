#include "stdafx.h"
#include "GameLogicManager.h"


GameLogicManager::GameLogicManager(Game* g) : game(g)
{
}


GameLogicManager::~GameLogicManager()
{
}

void GameLogicManager::Init(std::shared_ptr<Player> p, Map* m)
{
	player = p;
	map = m;
	players.Add(player->entityID, player);
}

void GameLogicManager::draw(sf::RenderWindow& renderWindow) {
	this->players.draw(renderWindow);
	this->bullets.draw(renderWindow);
	this->enemies.draw(renderWindow);
	
	if (this->game->switches.showPath)
	{
		this->drawTestPath();
	}
}

void GameLogicManager::drawTestPath()
{
	int i = 0;
	for (std::deque<sf::Vector2f>::const_iterator it = testPath.begin(); it != testPath.end(); ++it) {
		sf::Text text(std::to_string(i++), this->game->fonts["main_font"], 12);
		text.setPosition(*it);
		sf::CircleShape circle(5);
		circle.setOrigin(5, 5);
		circle.setFillColor(sf::Color::Black);
		circle.setPosition(*it);
		this->game->window.draw(circle);
		this->game->window.draw(text);
	}
}

void GameLogicManager::update(float dt) {
	this->players.update(dt);
	this->bullets.update(dt);
	this->enemies.update(dt);

	std::unordered_map<Guid, std::pair<sf::IpAddress, unsigned short>>::const_iterator remotesIt;
	for (remotesIt = this->game->networkmgr.networkGameObjects.begin(); remotesIt != this->game->networkmgr.networkGameObjects.end(); remotesIt++)
	{
		bool found = false;
		for (auto& o : this->players.objects)
		{
			if (remotesIt->first == o.second->entityID)
			{
				found = true;
				break;
			}
		}
		if (remotesIt->first == this->player->entityID)
		{
			found = true;
		}

		if (found == false)
		{
			this->players.Add(remotesIt->first, std::make_shared<Player>(game, new NetworkInput(game), remotesIt->first));
		}
	}

	GuidGenerator guidGen;
	for (auto& o : players.objects)
	{
		bullets.Add(o.second->weapon->get()->spawnedEntities);
			

		auto it = this->game->networkmgr.timeOut.find(o.first);
		if (it != this->game->networkmgr.timeOut.end() && it->second >= PLAYER_TIMEOUT)
		{
			o.second->isDeletable = true;

			this->game->networkmgr.timeOut.erase(it);
			this->game->networkmgr.networkGameObjects.erase(o.first);
		}
	}

	this->collisionManager.Collide<Player>(dt, players.objects, *map);
	this->collisionManager.Collide<Projectile>(dt, bullets.objects, *map);
	this->collisionManager.Collide<Enemies>(dt, enemies.objects, *map);
	this->collisionManager.Collide<Enemies, Projectile>(dt, enemies.objects, bullets.objects);
	if (!this->game->switches.playerInvincible)
		this->collisionManager.Collide<Projectile>(dt, bullets.objects, player);

	for (auto& enemy : enemies.objects)
	{
		//if (enemy.second->targetList.empty())
		//{
		//	testPath = map->pathfinder.searchPath(enemy.second->getPosition(), player->getPosition());
		//	enemy.second->moveToTarget(testPath);
		//}
		enemy.second->updateVisibleEnemies(this->players);
		enemy.second->updateVisibleProjectiles(this->bullets);
		if(enemy.second->target != nullptr)
		if (!V2Tools::inLineOfSight_againstPolygon(map->polygons, enemy.second->getPosition(), enemy.second->target->getPosition()))
		{
			enemy.second->enterState<IdleState>();
		}
		//	enemy.second->shootAtTarget(player->getPosition());
		//	
		//}
		//else
		//{
		//	enemy.second->isAttacking = false;
		//}
		bullets.Add(enemy.second->weapon.spawnedEntities);
	}
}


std::vector<std::shared_ptr<IGameEntity>> GameLogicManager::getVisibleObjects(std::shared_ptr<IGameEntity> ent, const GameObjectManager<IGameEntity>& toCheckAgainst )
{
	std::vector<std::shared_ptr<IGameEntity>> returnValue;
	for (auto& e : toCheckAgainst.objects)
	{
		if (V2Tools::inLineOfSight_againstPolygon(map->polygons, e.second->getPosition(), ent->getPosition()))
		{
			returnValue.emplace_back(e.second);
		}
	}
	return returnValue;
}

#pragma once
#include "Map.h"
#include "IAtomicEntity.h"

#include <unordered_map>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	template<typename TypeT> 
	void Collide(float dt, std::unordered_map<Guid, std::shared_ptr<TypeT>>& objects, Map& map);
	template<typename TypeT>
	void Collide(float dt, std::unordered_map<Guid, std::shared_ptr<TypeT>>& objects, std::shared_ptr<IAtomicEntity> entity);
	template<typename TypeT, typename TypeU>
	void Collide(float dt, std::unordered_map<Guid, std::shared_ptr<TypeT>>& objects, std::unordered_map<Guid, std::shared_ptr<TypeU>>& entities);
};

template<typename TypeT>
void CollisionManager::Collide(float dt, std::unordered_map<Guid, std::shared_ptr<TypeT>>& objects, Map& map) 
{
	typename std::unordered_map<Guid, std::shared_ptr<TypeT>>::const_iterator object = objects.begin();
	while (object != objects.end())
	{
		std::unordered_set<unsigned int> indices = map.grid.getCollisionObjects(object->second->getPosition());
		map.setDrawableHitboxes(indices);
		std::unordered_set<unsigned int>::const_iterator it;
		for (it = indices.begin(); it != indices.end(); ++it)
		{
			object->second->collide(map.hitboxes[*it], 0, dt);
		}
		++object;
	}
}

template<typename TypeT>
void CollisionManager::Collide(float dt, std::unordered_map<Guid, std::shared_ptr<TypeT>>& objects, std::shared_ptr<IAtomicEntity> entity) 
{
	typename std::unordered_map < Guid, std::shared_ptr<TypeT>> ::const_iterator object = objects.begin();
	while (object != objects.end())
	{
		//std::unordered_set<unsigned int> indices = map.grid.getCollisionObjects(object->second->getPosition());
		//std::unordered_set<unsigned int>::const_iterator it;
		//for (it = indices.begin(); it != indices.end(); ++it)
		//{
		entity->collide(*object->second, 1, dt);
		//object->second->collide(map.hitboxes[*it], 0, dt);
		//}
		++object;
	}
}

template<typename TypeT, typename TypeU>
void CollisionManager::Collide(float dt, std::unordered_map<Guid, std::shared_ptr<TypeT>>& objects, std::unordered_map<Guid, std::shared_ptr<TypeU>>& entities) {
	for (auto& object : objects)
	{
		for (auto& entity : entities)
		{
			object.second->collide(*entity.second, 1, dt);
		}
	}
}


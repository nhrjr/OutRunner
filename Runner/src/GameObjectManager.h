#pragma once
#include "Map.h"
#include "IAtomicEntity.h"
#include <unordered_map>

template<typename T>
class GameObjectManager
{
	static_assert(std::is_base_of<IAtomicEntity, T>::value, "GameObjectManager<T> - T must be derived from IAtomicEntity");
public:
	GameObjectManager();
	~GameObjectManager();

	void Add( Guid entityID, const std::shared_ptr<T>& gameObject);
	void Add(const std::shared_ptr<T>& gameObject);
	void Add(const std::vector<std::shared_ptr<T>>& gameObjects);
	void Remove(Guid entityID);
	int GetObjectCount() const;

	std::shared_ptr<T> Get(Guid entityID) const;

	void draw(sf::RenderWindow& renderWindow);
	void update(float dt);

	std::unordered_map<Guid, std::shared_ptr<T>> objects;

};

template<typename T>
GameObjectManager<T>::GameObjectManager()
{
}

template<typename T>
GameObjectManager<T>::~GameObjectManager()
{
}

template<typename T>
void GameObjectManager<T>::Add(Guid entityID, const std::shared_ptr<T>& gameObject) {
	objects.insert(std::make_pair(entityID, gameObject));
}

template<typename T>
void GameObjectManager<T>::Add(const std::shared_ptr<T>& gameObject) {
	objects.insert(std::make_pair(gameObject->entityID, gameObject));
}

template<typename T>
void GameObjectManager<T>::Add(const std::vector<std::shared_ptr<T>>& gameObjects)
{
	for (auto& obj : gameObjects) {
		objects.insert(std::make_pair(obj->entityID, obj));
	}
}

template<typename T>
void GameObjectManager<T>::Remove(Guid entityID)
{
	typename std::unordered_map<Guid,T*>::iterator results = objects.find(entityID);
	if (results != objects.end()) {
		delete results->second;
		objects.erase(results);
	}
}

template<typename T>
std::shared_ptr<T> GameObjectManager<T>::Get(Guid entityID) const 
{
	typename std::unordered_map<Guid, std::shared_ptr<T>>::const_iterator results = objects.find(entityID);
	if (results == objects.end()) {
		return NULL;
	}
	return results->second;
}

template<typename T>
int GameObjectManager<T>::GetObjectCount() const {
	return objects.size();
}

template<typename T>
void GameObjectManager<T>::draw(sf::RenderWindow& renderWindow) {
	for (auto& object : objects)
	{
		//objects.second->draw(renderWindow);
		renderWindow.draw(*object.second.get());
	}
}

template<typename T>
void GameObjectManager<T>::update(float dt) 
{
	typename std::unordered_map<Guid, std::shared_ptr<T>>::const_iterator it = objects.begin();
	while (it != objects.end())
	{
		it->second->update(dt);
		if (it->second->isDeletable == true)
		{
			it = objects.erase(it);
		}
		else
		{
			++it;
		}
	}
}


#pragma once
#include "Map.h"
#include "AStarSearch.h"
#include "MapHitbox.h"

#include <unordered_set>
#include <unordered_map>

//class Node;
//typedef std::unordered_map<size_t, Node, VectorHash> NodeHashMap;

enum class VertexType {
	CONVEX,
	CONCAVE,
	EDGE
};



typedef sf::Vector2f Vector;
typedef std::tuple<sf::Vector2f, VertexType, int> Vertex;

class MapTools
{
	
	VectorHash vectorHash;
	std::vector<MapHitbox> hitboxes;
	std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>> polygons;
	NodeHashMap nodes;

	Map* map;
	
public:
	MapTools(Map* map) : map(map) {}
	~MapTools() {}
	std::vector<MapHitbox> getHitboxes() { return hitboxes; };
	NodeHashMap getNodes() { return nodes; };
	std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>> getPolygons() {
		return polygons;
	};

	void constructMapObjects(std::vector<std::vector<sf::Vector2i>> regCoord);

	void createPolygons(std::vector<std::vector<sf::Vector2i>>& regCoord);
	void createMapHitBoxes(std::vector<std::vector<sf::Vector2i>>& regCoord);

	void orderHullPolygons();
	Vertex makeVertex(Vector vec, VertexType type, int orientation, int label, bool& makeExtendendPoint);
	sf::Vector2f getNextDirection(Vertex vertex);

	MapHitbox getMapHitbox(std::vector<sf::Vector2i>& reg);
	void createNodes();

};

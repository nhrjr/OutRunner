#pragma once
//#include "MapTools.h"
#include "Node.h"
#include <queue>
#include <map>







class PathFinder
{
	std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>>* polygons;
	NodeHashMap* nodes;

	sf::Vector2f startVec;
	sf::Vector2f endVec;
	Node startNode;
	Node endNode;

	VectorHash vectorHash;
	
public:
	
	PathFinder() {};
	PathFinder(NodeHashMap* n, std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>>* p) : nodes(n), polygons(p) {
		//createNodes();
	};

	//NodeHashMap* getNodes()
	//{
	//	return &nodes;
	//}

	

	std::deque<sf::Vector2f> searchPath(sf::Vector2f start, sf::Vector2f end);
	std::deque<sf::Vector2f> reconstructPath(std::unordered_map<Node, Node, NodeHash, NodeEqual>& path);

private:
	//void createNodes();

	std::unordered_map<Node, Node, NodeHash, NodeEqual> aStarSeach();
	float heuristic(Node*, Node*);
	void addPathNodes(sf::Vector2f& start, sf::Vector2f& end);
	void removePathNodes();
};


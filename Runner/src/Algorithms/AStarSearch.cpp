#include "stdafx.h"
#include "AStarSearch.h"
#include "V2Tools.h"
#include <fstream>

std::deque<sf::Vector2f> PathFinder::searchPath(sf::Vector2f a, sf::Vector2f b)
{
	removePathNodes();
	addPathNodes(a, b);
	
	std::unordered_map<Node, Node, NodeHash, NodeEqual> cameFrom = this->aStarSeach();

	
	
	return reconstructPath(cameFrom);
}

std::deque<sf::Vector2f> PathFinder::reconstructPath(std::unordered_map<Node, Node, NodeHash, NodeEqual>& path)
{
	std::deque<sf::Vector2f> travelList;
	travelList.emplace_front(endNode.position);
	Node previous = endNode;
	while (!path.empty())
	{
		auto it = path.find(previous);
		if (it == path.end()) break;
		travelList.emplace_front(it->second.position);
		previous = it->second;
		//path.erase(it)
	}
	return travelList;
}

void PathFinder::addPathNodes(sf::Vector2f& start, sf::Vector2f& end)
{
	startVec = start;
	endVec = end;
	startNode = Node(start);
	endNode = Node(end);
	
	nodes->emplace(start,Node(start));
	nodes->emplace(end,Node(end));
	NodeHashMap::iterator startIt = nodes->find(start);
	NodeHashMap::iterator endIt = nodes->find(end);

	Node* emergencyNode = &startIt->second;
	float emergencyDistance = V2Tools::distance(start,end);
	bool emergency = true;
	
	for(auto& node : *nodes)
	{
		float distance = V2Tools::distance(start, node.second.position);
		if (distance > 0.0f)
		{
			if (V2Tools::inLineOfSight_againstPolygon(*polygons, start, node.second.position))
			{
				//if (V2Tools::inLineOfSight_againstConvexHull(*polygons, start, node.second.position))
				//{
					startIt->second.neighborHashes.emplace_back(node.first);
					startIt->second.distance.emplace_back(distance);
				//}

			}
		}

		distance = V2Tools::distance(node.second.position, end);
		if (distance > 0.0f)
		{
			if (distance < emergencyDistance)
			{
				emergencyDistance = distance;
				emergencyNode = &node.second;
			}
			if (V2Tools::inLineOfSight_againstPolygon(*polygons, node.second.position, end))
			{
				//if (V2Tools::inLineOfSight_againstConvexHull(*polygons, node.second.position, end))
				//{
					emergency = false;
					node.second.neighborHashes.emplace_back(endIt->first);
					node.second.distance.emplace_back(distance);
				//}
			}
		}
	}
	if (emergency)
	{
		emergencyNode->neighborHashes.emplace_back(endIt->first);
		emergencyNode->distance.emplace_back(emergencyDistance);
	}
}

void PathFinder::removePathNodes()
{
	if (nodes->size() >= 2) {
		for(auto& node : *nodes)
		{
			if (!node.second.neighborHashes.empty()) {
				if (node.second.neighborHashes.back() == startVec || node.second.neighborHashes.back() == endVec)
				{
					node.second.neighborHashes.pop_back();
					node.second.distance.pop_back();
				}
			}
			node.second.distanceTravelled = 0;
		}
	}
	nodes->erase(startVec);
	nodes->erase(endVec);
}

float PathFinder::heuristic(Node* a, Node* b)
{
	return V2Tools::distance(a->position, b->position);
}

void drawNode(sf::Vector2f pos, sf::RenderWindow& window, sf::Color color)
{
	sf::CircleShape circle(5);
	circle.setOrigin(5, 5);
	circle.setFillColor(color);
	circle.setPosition(pos);
	window.draw(circle);

}

struct CompareNodePriority {
public:
	bool operator()(const Node& a, const Node& b) const {
		return a.priority > b.priority;
	}
};



std::unordered_map<Node, Node, NodeHash, NodeEqual> PathFinder::aStarSeach()
{
	std::priority_queue<Node, std::vector<Node>, CompareNodePriority> frontier;
	std::unordered_map<Node, Node, NodeHash, NodeEqual> cameFrom;
	NodeHashMap::iterator endIt = nodes->find(endVec);
	NodeHashMap::iterator startIT = nodes->find(startVec);
	frontier.push(startIT->second);

	while (!frontier.empty())
	{
		Node current = frontier.top();
		frontier.pop();
		if (current.position == endVec) break;

		for(int i = 0; i < current.neighborHashes.size(); ++i)
		{
			sf::Vector2f nextVec = current.neighborHashes[i];
			NodeHashMap::iterator next = nodes->find(nextVec);
			float new_distance = current.distanceTravelled + current.distance[i];

			if (next->second.distanceTravelled == 0 || new_distance < next->second.distanceTravelled)
			{
				next->second.distanceTravelled = new_distance;
				float priority = new_distance + heuristic(&next->second, &endIt->second);
				next->second.priority = priority;
				frontier.push(next->second);
				cameFrom[next->second] = current;
			}
		}
	}
	return cameFrom;
}




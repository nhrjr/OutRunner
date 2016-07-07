#pragma once
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>





class Node
{
public:
	sf::Vector2f position;
	//size_t hash;
	std::vector<sf::Vector2f> neighborHashes;
	std::vector<float> distance;
	float priority;
	float distanceTravelled;
	//bool specialNode;

	//Node(bool special) : priority(0), distanceTravelled(0), specialNode(false) {};
	Node() : priority(0), distanceTravelled(0) {};
	Node(sf::Vector2f p) : position(p), priority(0), distanceTravelled(0) {
	};
};

struct NodeHash {
	size_t operator() (const Node& vec) const {
		size_t v1 = std::hash<float>()(vec.position.x);
		size_t v2 = std::hash<float>()(vec.position.y);
		return (v1 ^ (v2 << 1));
	}
};

struct NodeEqual {
	bool operator() (const Node& a, const Node& b) const {
		return (a.position.x == b.position.x) && (a.position.y == b.position.y);
	}
};

struct VectorHash {
	size_t operator() (const sf::Vector2f& vec) const {
		size_t v1 = std::hash<float>()(vec.x);
		size_t v2 = std::hash<float>()(vec.y);
		return (v1 ^ (v2 << 1));
	}
};

struct VectorEqual {
	bool operator() (const sf::Vector2f& a, const sf::Vector2f& b) const {
		return (a.x == b.x) && (a.y == b.y);
	}
};



typedef std::unordered_map<sf::Vector2f, Node, VectorHash, VectorEqual> NodeHashMap;
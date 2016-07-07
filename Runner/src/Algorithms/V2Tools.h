#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>

namespace V2Tools
{

	sf::Vector2f normalize(const sf::Vector2f& v);
	float distance(const sf::Vector2f& a, const sf::Vector2f& b);
	int distance(const sf::Vector2i& a, const sf::Vector2i& b);
	float length(const sf::Vector2f& v);
	int length(const sf::Vector2i& v);
	float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b);
	float crossProduct(const sf::Vector2f& a, const sf::Vector2f& b);
	float det(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& center);

	bool doLineSegmentsCross(const sf::Vector2f& firstLineA, const sf::Vector2f& firstLineB, const sf::Vector2f& secondLineA, const sf::Vector2f& secondLineB,
		 bool considerConnectingPointsAsIntersect = true, bool considerCollinearOverlapAsIntersect = false);
	bool isPointOnLineSegment(const sf::Vector2f& lineA, const sf::Vector2f& lineB, const sf::Vector2f& point);
	bool isPointOnLine(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f a);
	//bool inLineOfSight(std::vector<sf::Vector2f>& polygon, const sf::Vector2f& a, const sf::Vector2f& b);
	bool inLineOfSight_againstPolygon(std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>>& polygons, const sf::Vector2f& a, const sf::Vector2f& b);
	bool inLineOfSight_againstConvexHull(std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>>& polygons, const sf::Vector2f& a, const sf::Vector2f& b);
	void writeVectorToStream(const sf::Vector2f& vec, std::ofstream& outputFile);
	void readVectorFromStream(sf::Vector2f& vec, std::ifstream& inputFile);

}




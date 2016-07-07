#pragma once
#include <SFML/Graphics.hpp>
#include "GameEntities/IGameEntity.h"
#include "Core/Game.h"

struct ProjectionRange
{
	float min, max;
};

struct colDrawHelper
{
	std::vector<sf::Vector2f> d1points;
	std::vector<sf::Vector2f> d2points;

	std::vector<sf::Vector2f> d1axis;
	std::vector<sf::Vector2f> d2axis;
};

namespace Collision
{

	sf::Vector2f SATCollision(const IGameEntity* d1,const IGameEntity* d2/*, colDrawHelper& helper*/);

	void draw(sf::RenderWindow& window, const colDrawHelper& helper);
	//void draw(sf::RenderWindow& window);
	//float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b);
	//float projectionsOverlap(ProjectionRange& d1range, ProjectionRange& d2range);
	//ProjectionRange generateProjectionRangeOnAxis(const std::vector<sf::Vector2f>& points, const sf::Vector2f& axis);
	//std::vector<sf::Vector2f> getPointsFromShape(IGameEntity* entity);
	//std::vector<sf::Vector2f> getAxesFromPoints(const std::vector<sf::Vector2f>& points);
	//sf::Vector2f shortestAxis(sf::Vector2f& point, std::vector<sf::Vector2f>& points);
	//sf::Vector2f intersectionPoint(sf::Vector2f axis, float radius);

};




#include "stdafx.h"
#include "SAT.h"
#include <math.h>
#include <algorithm>
#include "GameSettingConstants.h"
#include "MapTools.h"
#include "V2Tools.h"

#include <iostream>



float projectionsOverlap(const ProjectionRange& d1range, const ProjectionRange& d2range)
{
	return std::min(d1range.max, d2range.max) - std::max(d1range.min, d2range.min);
}

ProjectionRange generateProjectionRangeOnAxis(const std::vector<sf::Vector2f>& points, const sf::Vector2f& axis)
{
	ProjectionRange result;
	result.max = result.min = V2Tools::dotProduct(points[0], axis);

	float temp;

	for (int i = 1; i < points.size(); ++i)
	{
		temp = V2Tools::dotProduct(points[i], axis);
		if (temp < result.min) result.min = temp;
		else if (temp > result.max) result.max = temp;
	}
	return result;
}

sf::Vector2f shortestAxis(const sf::Vector2f& point, const std::vector<sf::Vector2f>& points)
{
	std::map<float, sf::Vector2f> result;
	for (auto p : points)
	{
		result.emplace(std::make_pair(V2Tools::distance(point, p), p - point));
	}
	return V2Tools::normalize(result.begin()->second);
}

sf::Vector2f nearestEdgeNormal(const sf::Vector2f& point, const std::vector<sf::Vector2f>& points)
{
	std::map<float, sf::Vector2f> result;
	for (auto p : points)
	{
		result.emplace(std::make_pair(V2Tools::distance(point, p), p - point));
	}
	sf::Vector2f returnValue;
	std::map<float, sf::Vector2f>::const_iterator itr = result.begin();
	sf::Vector2f first = itr->second;
	itr++;
	sf::Vector2f second = itr->second;
	returnValue = V2Tools::normalize(second - first);
	returnValue = sf::Vector2f(-returnValue.y, returnValue.x);
	return returnValue;
}

sf::Vector2f intersectionPoint(sf::Vector2f axis, float radius)
{
	return axis * radius;
}

std::vector<sf::Vector2f> getPointsFromShape(const IGameEntity* entity)
{
	unsigned int numberOfPoints = entity->getPointCount();
	std::vector<sf::Vector2f> result;

	sf::Transform transform = entity->getTransform();
	for (unsigned int i = 0; i < numberOfPoints; ++i)
	{
		result.emplace_back(transform.transformPoint(entity->getPoint(i)));
	}
	return result;
}

bool isParallel(const sf::Vector2f& a, const std::vector<sf::Vector2f>& vec)
{
	if (vec.size() == 0) return false;
	for (auto v : vec)
	{
		float product = std::abs(V2Tools::dotProduct(a, v));
		if (product == 1.0f)
		{
			return true;
		}
	}
	return false;
}

std::vector<sf::Vector2f> getAxesFromPoints(const std::vector<sf::Vector2f>& points)
{
	std::vector<sf::Vector2f> result;

	sf::Vector2f edge;
	for (unsigned i = 0; i < points.size(); ++i)
	{
		edge = points[(i + 1) % points.size()] - points[i];
		edge = V2Tools::normalize(edge);

		if (!isParallel(edge, result))
		{
			result.push_back(edge);
		}
	}
	for (auto& r : result)
	{
		r = sf::Vector2f(-r.y, r.x);
	}
	return result;
}






namespace Collision
{
	sf::Vector2f SATCollision(const IGameEntity* d1,const IGameEntity* d2/*, colDrawHelper& helper*/)
	{
		std::vector<sf::Vector2f> d1points = getPointsFromShape(d1);
		std::vector<sf::Vector2f> d2points = getPointsFromShape(d2);

		if (d1points.size() <= 1 || d2points.size() <= 1) return sf::Vector2f(0, 0);

		sf::Vector2f directionVector = sf::Vector2f(d2->getPosition().x - d1->getPosition().x, d2->getPosition().y - d1->getPosition().y);

		std::vector<sf::Vector2f> d1axis = getAxesFromPoints(d1points);
		std::vector<sf::Vector2f> d2axis = getAxesFromPoints(d2points);

		//std::vector<sf::Vector2f> d1axis;
		//std::vector<sf::Vector2f> d2axis;
		/*helper.d1points = d1points;
		helper.d2points = d2points;
		helper.d1axis = d1axis;
		helper.d2axis = d2axis;*/

		//if (d1points.size() == 1)
		//{
		//	
		//	sf::Vector2f tmp = nearestEdgeNormal(d1points[0], d2points);
		//	if (V2Tools::dotProduct(directionVector, tmp) < 0) tmp = -tmp;
		//	d1axis.emplace_back(tmp);
		//
		//	//d1axis.emplace_back(shortestAxis(d1points[0], d2points));
		//	
		//	d1points.emplace_back(d1points[0] + intersectionPoint(d1axis[0], d1->getMinDistance()));
		//	d2axis = getAxesFromPoints(d2points);
		//}
		//else if (d2points.size() == 1)
		//{
		//	sf::Vector2f tmp = nearestEdgeNormal(d2points[0], d1points);
		//	if (V2Tools::dotProduct(directionVector, tmp) > 0) tmp = -tmp;
		//	//d2axis.emplace_back(shortestAxis(d2points[0], d1points));
		//	d2axis.emplace_back(tmp);
		//	d2points.emplace_back(d2points[0] + intersectionPoint(d2axis[0], d2->getMinDistance()));
		//	d1axis = getAxesFromPoints(d1points);
		//}
		//else
		//{
		//	d1axis = getAxesFromPoints(d1points);
		//	d2axis = getAxesFromPoints(d2points);
		//}

		std::vector<sf::Vector2f> allAxis;
		allAxis.insert(allAxis.end(), d1axis.begin(), d1axis.end());
		allAxis.insert(allAxis.end(), d2axis.begin(), d2axis.end());

		ProjectionRange d1range;
		ProjectionRange d2range;

		float minOverlap = SAT_MAX_OVERLAP;
		sf::Vector2f minAxis;

		for (auto axis : allAxis) {
			d1range = generateProjectionRangeOnAxis(d1points, axis);
			d2range = generateProjectionRangeOnAxis(d2points, axis);

			float overlap = projectionsOverlap(d1range, d2range);

			if (overlap < 0.0f) {
				return sf::Vector2f(0, 0);
			}
			else {
				if (overlap < minOverlap) {
					minOverlap = overlap;
					minAxis = axis;
				}
			}
		}

		if (directionVector.x*minAxis.x + directionVector.y * minAxis.y > 0)
		{
			minAxis.x = -minAxis.x;
			minAxis.y = -minAxis.y;
		}

		return minAxis;
	}
}



	void Collision::draw(sf::RenderWindow& window, const colDrawHelper& helper)
	{
		std::vector<sf::CircleShape> dots;
		std::vector<sf::RectangleShape> axis;
		int counter = 0;
		for (auto& d : helper.d1points) {
			sf::CircleShape tmp(3);
			tmp.setFillColor(sf::Color::Red);
			tmp.setOrigin(3, 3);
			tmp.setPosition(d);
			dots.emplace_back(tmp);

			float overlap = projectionsOverlap(generateProjectionRangeOnAxis(helper.d1points, helper.d1axis[0]), generateProjectionRangeOnAxis(helper.d2points, helper.d1axis[0]));
			float angle = atan2(helper.d1axis[0].y, helper.d1axis[0].x) * 180 / M_PI;

			sf::RectangleShape tmp2(sf::Vector2f(overlap, 2));
			tmp2.setFillColor(sf::Color::Blue);
			tmp2.setOrigin(2, 0);
			tmp2.setPosition(d);
			tmp2.setRotation(angle);
			axis.emplace_back(tmp2);
			counter++;
		}
		counter = 0;
		for (auto& d : helper.d2points) {
			sf::CircleShape tmp(3);
			tmp.setFillColor(sf::Color::Red);
			tmp.setOrigin(3, 3);
			tmp.setPosition(d);
			dots.emplace_back(tmp);

			if (counter < helper.d2axis.size())
			{
				float overlap = projectionsOverlap(generateProjectionRangeOnAxis(helper.d1points, helper.d2axis[counter]), generateProjectionRangeOnAxis(helper.d2points, helper.d2axis[counter]));
				float angle = atan2(helper.d2axis[counter].y, helper.d2axis[counter].x) * 180 / M_PI;

				sf::RectangleShape tmp2(sf::Vector2f(overlap, 2));
				tmp2.setFillColor(sf::Color::Blue);
				tmp2.setOrigin(2, 0);
				tmp2.setPosition(d);
				tmp2.setRotation(angle);
				axis.emplace_back(tmp2);
			}

			
			counter++;
		}

		for (auto& dot : dots) {
			window.draw(dot);
		}
		for (auto& ax : axis) {
			window.draw(ax);
		}
	}



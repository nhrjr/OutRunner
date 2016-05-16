#include "stdafx.h"
#include "V2Tools.h"

#include <cmath>


namespace V2Tools
{
	//bool V2Tools::inLineOfSight(std::vector<sf::Vector2f>& polygon, const sf::Vector2f& a, const sf::Vector2f& b)
	//{
	//	//for (auto& vertex : polygon)
	//	//{
	//		for (int i = 0; i < polygon.size(); ++i){
	//		
	//			if (V2Tools::doLineSegmentsCross(polygon[(i + 1) % polygon.size()], polygon[i], a, b))
	//				return false;
	//		}
	//	//}
	//	return true;
	//}

	bool inLineOfSight_againstConvexHull(std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>>& polygons, const sf::Vector2f& a, const sf::Vector2f& b)
	{
		for (auto& polygon : polygons)
		{
			for (int i = 0; i < polygon.first.size(); ++i)
			{
				if (V2Tools::doLineSegmentsCross(polygon.first[(i + 1) % polygon.first.size()], polygon.first[i], a, b,false,false))
					return false;
			}
		}
		return true;
	}

	bool inLineOfSight_againstPolygon(std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>>& polygons, const sf::Vector2f& a, const sf::Vector2f& b)
	{
		for (auto& polygon : polygons)
		{
			for (int i = 0; i < polygon.second.size(); ++i)
			{
				if (V2Tools::doLineSegmentsCross(polygon.second[(i + 1) % polygon.second.size()], polygon.second[i], a, b,true,false))
					return false;
			}
		}
		return true;
	}

	float length(const sf::Vector2f& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	int length(const sf::Vector2i& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	sf::Vector2f normalize(const sf::Vector2f& v)
	{
		sf::Vector2f result = v;
		if (v != sf::Vector2f(0, 0)) {
			float len = length(v);
			return result /= len;
		}
		else
			return result;
	}

	float distance(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return length(a - b);
	}

	int distance(const sf::Vector2i& a, const sf::Vector2i& b)
	{
		return length(a - b);
	}


	float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x*b.x + a.y*b.y;
	}

	float crossProduct(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	bool isPointOnLine(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f a)
	{
		sf::Vector2f vec1 = p2 - p1;
		sf::Vector2f vec2 = a - p1;
		if (abs(V2Tools::crossProduct(vec1, vec2)) < 0.001f)
			return true;
		else
			return false;
	}

	bool doLineSegmentsCross(const sf::Vector2f& p, const sf::Vector2f& p2, const sf::Vector2f& q, const sf::Vector2f& q2,
		bool considerConnectingPointsAsIntersect, bool considerCollinearOverlapAsIntersect)
		//bool LineSegementsIntersect(Vector p, Vector p2, Vector q, Vector q2,
		//	out Vector intersection, bool considerCollinearOverlapAsIntersect = false)
	{
		//sf::Vector2f intersection;

		auto r = p2 - p;
		auto s = q2 - q;
		auto rxs = V2Tools::crossProduct(r, s);
		auto qpxr = V2Tools::crossProduct((q - p), r);

		// If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
		if (rxs == 0.0f && qpxr == 0.0f)
		{
			// 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
			// then the two lines are overlapping,
			if (considerCollinearOverlapAsIntersect)
				if (0 <= V2Tools::dotProduct((q - p), r) && V2Tools::dotProduct((q - p), r) <= V2Tools::dotProduct(r, r)
					|| 0 <= V2Tools::dotProduct((p - q), s) && V2Tools::dotProduct((p - q), s) <= V2Tools::dotProduct(s, s))
					return true;

			// 2. If neither 0 <= (q - p) * r = r * r nor 0 <= (p - q) * s <= s * s
			// then the two lines are collinear but disjoint.
			// No need to implement this expression, as it follows from the expression above.
			return false;
		}

		// 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
		if (rxs == 0.0f && qpxr != 0.0f)
			return false;

		// t = (q - p) x s / (r x s)
		auto t = V2Tools::crossProduct((q - p), s) / rxs;

		// u = (q - p) x r / (r x s)

		auto u = V2Tools::crossProduct((q - p), r) / rxs;

		// 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
		// the two line segments meet at the point p + t r = q + u s.
		if (considerConnectingPointsAsIntersect)
		{
			if (rxs != 0.0f && (0 <= t && t <= 1) && (0 <= u && u <= 1))
			{
				// We can calculate the intersection point using either t or u.
				//intersection = p + t*r;

				// An intersection was found.
				return true;
			}
		}
		else
		{
			if (rxs != 0.0f && (0 < t && t < 1) && (0 < u && u < 1))
			{
				// We can calculate the intersection point using either t or u.
				//intersection = p + t*r;

				// An intersection was found.
				return true;
			}
		}
		

		// 5. Otherwise, the two line segments are not parallel but do not intersect.
		return false;
	}

	bool isPointOnLineSegment(const sf::Vector2f& lineA, const sf::Vector2f& lineB, const sf::Vector2f& point)
	{
		return abs(distance(lineA, point) + distance(lineB, point) - distance(lineA, lineB)) < 0.5f;
	}

	float det(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& center)
	{
		return (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
	}


	void writeVectorToStream(const sf::Vector2f& vec, std::ofstream& outputFile)
	{
		outputFile.write((char*)&vec.x, sizeof(float));
		outputFile.write((char*)&vec.y, sizeof(float));
	}
	void readVectorFromStream(sf::Vector2f& vec, std::ifstream& inputFile)
	{
		inputFile.read((char*)&vec.x, sizeof(float));
		inputFile.read((char*)&vec.y, sizeof(float));
	}

}

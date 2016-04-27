#include "stdafx.h"
#include "MapTools.h"
#include "MapHitbox.h"
#include "GameSettingConstants.h"
#include "V2Tools.h"
//#include "AStarSearch.h"

#include <array>


void sortCoordinateVector(std::vector<sf::Vector2i>& reg)
{
	std::sort(reg.begin(), reg.end(), [](const sf::Vector2i& a, const sf::Vector2i& b) {
		if (a.x < b.x) return true;
		if (a.x == b.x) {
			if (a.y < b.y) return true;
		}
		return false;
	});
}

sf::Vector2i getVertical(std::vector<sf::Vector2i>::const_iterator& it, std::vector<sf::Vector2i>& reg, int line_x)
{
	int counter = 0;
	sf::Vector2i tmp = *it;
	while (it != reg.end() && it->x == line_x) {
		int distance = it->y - tmp.y;
		if (distance <= 1)
		{
			tmp = *(it);
			if (++counter >= HITBOX_MAX_SIZE) break;
		}
		else {
			break;
		}
		++it;
		
			
	}
	return tmp;
}




bool less(const sf::Vector2f& a,const sf::Vector2f& b,const sf::Vector2f& center)
{
	if (a.x - center.x >= 0 && b.x - center.x < 0)
		return true;
	if (a.x - center.x < 0 && b.x - center.x >= 0)
		return false;
	if (a.x - center.x == 0 && b.x - center.x == 0) {
		if (a.y - center.y >= 0 || b.y - center.y >= 0)
			return a.y > b.y;
		return b.y > a.y;
	}

	// compute the cross product of vectors (center -> a) x (center -> b)
	float det = V2Tools::det(a, b, center);
	//int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
	if (det < 0)
		return true;
	if (det > 0)
		return false;

	// points a and b are on the same line from the center
	// check which point is closer to the center
	int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
	int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
	return d1 > d2;
}

std::vector<sf::Vector2f> orderVertices(std::vector<sf::Vector2f> polygon)
{
	sf::Vector2f center(0, 0);
	for (auto& p : polygon)
	{
		center += p;
	}
	center /= (float)polygon.size();

	std::sort(polygon.begin(), polygon.end(), [center](const sf::Vector2f& a, const sf::Vector2f& b) {return less(a, b, center); });
	return polygon;
}

std::vector<sf::Vector2f> findConvexVertices(const std::vector<sf::Vector2f>& p)
{
	std::vector<sf::Vector2f> polygon = orderVertices(p);
	//std::vector<sf::Vector2f> polygon(p.begin(), p.end());
	std::vector<sf::Vector2f> returnValue;
	for (int i = 0; i < polygon.size(); ++i)
	{
		sf::Vector2f current = polygon[i];
		int nextIndex = (i + 1) % polygon.size();
		sf::Vector2f next = polygon[nextIndex];
		int previousIndex = i == 0 ? polygon.size() - 1 : i - 1;
		sf::Vector2f previous = polygon[previousIndex];

		sf::Vector2f left(previous - current);
		sf::Vector2f right(next - current);
		float cross = V2Tools::crossProduct(left, right);
		if ( cross > 0.0f)
		{
			returnValue.push_back(current);
		}
	}
	return returnValue;
}

MapHitbox MapTools::getMapHitbox(std::vector<sf::Vector2i>& reg)
{
	std::vector<sf::Vector2i> tmp(2);
	tmp[0] = *reg.begin();
	tmp[1] = *reg.begin();

	std::vector<sf::Vector2i>::const_iterator it = reg.begin();

	tmp[1] = getVertical(it, reg, tmp[0].x);

	int x_up = tmp[1].x;
	int counter = 0;
	while (it != reg.end()) {
		int distance = it->x - tmp[1].x;
		
		if (it->y == tmp[0].y) {
			if (distance <= 1) {
				x_up = it->x;
				sf::Vector2i t = getVertical(it, reg, x_up);
				if (t.y == tmp[1].y) {
					tmp[1] = t;
					if (++counter >= HITBOX_MAX_SIZE-1) break;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		else {
		++it;
		}
	}

	auto func = [&tmp](const sf::Vector2i& a) {
		if (a.x <= tmp[1].x && a.x >= tmp[0].x) {
			if (a.y <= tmp[1].y && a.y >= tmp[0].y) {
				return true;
			}
		}
		return false;
	};
	reg.erase(std::remove_if(reg.begin(), reg.end(), func), reg.end());

	sf::Vector2f nw = sf::Vector2f(tmp[0].x * GAME_TILESIZE, tmp[0].y * GAME_TILESIZE);
	sf::Vector2f se = sf::Vector2f(tmp[1].x * GAME_TILESIZE + GAME_TILESIZE, tmp[1].y * GAME_TILESIZE + GAME_TILESIZE);

	MapHitbox rec = MapHitbox(se - nw);
	rec.setPosition(nw);
	return rec;
}

void addUniqueVector(std::vector<sf::Vector2f>& vec, sf::Vector2f point)
{
	for (auto& v : vec)
	{
		if (V2Tools::distance(v, point) < 0.5f)
		{
			return;
		}
	}
	vec.push_back(point);
}
	
//void MapTools::constructMapObjects(std::vector<std::vector<sf::Vector2i>>& regCoord)
//{
//	std::vector<sf::Vector2f> polygon;
//	for (auto& reg : regCoord)
//	{
//		polygon.clear();
//		sortCoordinateVector(reg);
//		while (reg.size() > 0) {
//			hitboxes.emplace_back(getMapHitbox(reg));
//			for (int i = 0; i < hitboxes.back().getPointCount(); ++i)
//			{
//				addUniqueVector(polygon, hitboxes.back().getPoint(i) + hitboxes.back().getPosition());
//			}
//		}
//		polygons.emplace_back(findConvexVertices(polygon));
//	}
//}

void MapTools::createMapHitBoxes(std::vector<std::vector<sf::Vector2i>>& regCoord)
{
	for (auto& region : regCoord)
	{
		sortCoordinateVector(region);
		while (region.size() > 0) {
			hitboxes.emplace_back(getMapHitbox(region));
		}
	}
}




void MapTools::constructMapObjects(std::vector<std::vector<sf::Vector2i>>& regCoord)
{
	createPolygons(regCoord);

	createNodes();

	createMapHitBoxes(regCoord);
}

void countPointFrequency(std::vector<std::pair<int, sf::Vector2f>>& pointCounter, sf::Vector2f point)
{
	for (auto& v : pointCounter)
	{
		if (V2Tools::distance(v.second, point) == 0.0f)
		{
			v.first++;
			return;
		}
	}
	pointCounter.push_back(std::pair<int, sf::Vector2f>(0, point));
}

void MapTools::createPolygons(std::vector<std::vector<sf::Vector2i>>& regCoord)
{
	//std::vector<std::vector<sf::Vector2f>> pointsInside;
	int i = 0;
	for (auto& region : regCoord)
	{
		std::vector<std::pair<int, sf::Vector2f>> polygonPointCounter;
		
		//pile all tile points onto its regional polygon, and count the number of overlapping points
		for (auto& tile : region)
		{
			
			sf::Vector2f one = sf::Vector2f(tile.x*GAME_TILESIZE, tile.y*GAME_TILESIZE);
			countPointFrequency(polygonPointCounter, one);
			//pointsInside[i].emplace_back(one + sf::Vector2f(GAME_TILESIZE / 2, GAME_TILESIZE / 2));
			//polygonPointCounter.push_back(std::pair<int,sf::Vector2f>(0,one));

			sf::Vector2f two = sf::Vector2f(tile.x*GAME_TILESIZE + GAME_TILESIZE, tile.y*GAME_TILESIZE);
			sf::Vector2f three = sf::Vector2f(tile.x*GAME_TILESIZE + GAME_TILESIZE, tile.y*GAME_TILESIZE + GAME_TILESIZE);
			sf::Vector2f four = sf::Vector2f(tile.x*GAME_TILESIZE, tile.y*GAME_TILESIZE + GAME_TILESIZE);
			countPointFrequency(polygonPointCounter, two);
			countPointFrequency(polygonPointCounter, three);
			countPointFrequency(polygonPointCounter, four);
		}
		// get appropriate points ( 0 == only 1 point counted (convex points), 2 == 3 points counted (convex + concave points)
		std::vector<sf::Vector2f> convexPoly;
		std::vector<sf::Vector2f> hullPoly;
		for (auto& point : polygonPointCounter)
		{
			if (point.first == 0)
			{
				convexPoly.push_back(point.second);
				hullPoly.push_back(point.second);
			}
			if (point.first == 1)
			{
				//convexPoly.push_back(point.second);
				hullPoly.push_back(point.second);
			}
			if (point.first == 2)
			{
				//convexPoly.push_back(point.second);
				hullPoly.push_back(point.second);
			}
			//hullPoly.push_back(point.second);


			//if(point.first == 2)
			//	polygon.push_back(point.second);
		}

		polygons.emplace_back(std::make_pair(convexPoly,hullPoly));

		polygonPointCounter.clear();
		++i;
	}

	orderHullPolygons();
}




void MapTools::orderHullPolygons()
{

	float alpha = GAME_TILESIZE * 0.5f;
	Vector SE = Vector(alpha, alpha);
	Vector SW = Vector(-alpha, alpha);
	Vector NW = Vector(-alpha, -alpha);
	Vector NE = Vector(alpha, -alpha);
	std::array<Vector, 4> orientation = { NW, NE, SE, SW };

	int j = 0;
	for (auto& polygon : polygons)
	{
		std::vector<Vector> orderedHull;
		std::vector<Vector> orderedConvexHull;

		Vector startCorner = polygon.second[0];
		Vertex startVertex;
		Vertex wanderer;
		std::array<int, 4> labels;
		int i = 0;
		int labelSum = 0;
		int label = 0;
		for (auto or : orientation)
		{
			Vector tileVector = startCorner + or;
			auto tmp = map->getCurrentTile(tileVector);
			labels[i] = tmp->region[0];
			if (labels[i] != 0) label = labels[i];
			labelSum += labels[i];
			++i;
		}
		if (labelSum == 3 * label) {
			//concave point
			for (int k = 0; k < 4; ++k) {
				if (labels[k] == 0)
				{
					map->debugTags.push_back("0_A" + std::to_string(k));
					startVertex = std::make_tuple(startCorner, VertexType::CONCAVE, k);
				}

			}
		}
		else {

			//if (labelSum == label){
				//convex point
			for (int k = 0; k < 4; ++k) {
				if (labels[k] == label)
				{
					int o = (k + 3) % 4;
					map->debugTags.push_back("0_X" + std::to_string(o));
					bool makeExtendedPoint = false;
					startVertex = makeVertex(startCorner, VertexType::CONVEX, o, label, makeExtendedPoint);
					//startVertex = std::make_tuple(startCorner, VertexType::CONVEX, k);
					if (makeExtendedPoint == true)
					{
						int l = std::get<2>(startVertex);
						Vector extendedPoint = std::get<0>(startVertex);
						switch (l) {
						case 0:
						{
							extendedPoint += SE;
							break;
						}
						case 1:
						{
							extendedPoint += SW;
							break;
						}
						case 2:
						{
							extendedPoint += NW;
							break;
						}
						case 3:
						{
							extendedPoint += NE;
							break;
						}

						}
						orderedConvexHull.push_back(extendedPoint);
					}
				}
			}
		}
	/*	}else 
		}else{
			std::cout << "HELP in orderHullPolygons()" << std::endl;
		}*/
		wanderer = startVertex;
		orderedHull.push_back(std::get<0>(wanderer));
		Vector nextDirection = getNextDirection(wanderer);
		Vector wandererPoint = std::get<0>(wanderer);


		//while (orderedHull.size() != polygon.second.size())
		for (int m = 0; m < polygon.second.size(); ++m)
		{
			std::map<float, Vector> mapOfCandidates;
			for (int k = 0; k < polygon.second.size(); ++k)
			{
				if (V2Tools::isPointOnLine(wandererPoint, wandererPoint + nextDirection, polygon.second[k]))
				{
					float dp = V2Tools::dotProduct(nextDirection, polygon.second[k] - wandererPoint);
					if (dp > 0)
					{
						mapOfCandidates.emplace(V2Tools::distance(wandererPoint, polygon.second[k]),polygon.second[k]);
					}
				}
			}
			
			while(!mapOfCandidates.empty())
			{
				auto candidate = mapOfCandidates.begin();

				//auto it = mapOfCandidates.begin();

				wandererPoint = candidate->second;
				if (wandererPoint == startCorner) break;
				bool makeExtendedPoint = false;
				Vertex tmp = makeVertex(wandererPoint, std::get<1>(wanderer), std::get<2>(wanderer), label, makeExtendedPoint);
				if (std::get<1>(tmp) == VertexType::EDGE)
				{
					mapOfCandidates.erase(candidate);
					continue;
				}
				else
				{
					mapOfCandidates.clear();
				}
				wanderer = tmp;
				nextDirection = getNextDirection(wanderer);
				orderedHull.push_back(std::get<0>(wanderer));

				if (makeExtendedPoint == true)
				{
					int k = std::get<2>(wanderer);
					Vector extendedPoint = std::get<0>(wanderer);
					switch (k) {
						case 0:
						{
							extendedPoint += SE;
							break;
						}
						case 1:
						{
							extendedPoint += SW;
							break;
						}
						case 2:
						{
							extendedPoint += NW;
							break;
						}
						case 3:
						{
							extendedPoint += NE;
							break;
						}
						default:
							break;

					}
					orderedConvexHull.push_back(extendedPoint);
				}
			}
		}
		//auto it = std::unique(orderedHull.begin(), orderedHull.end(),VectorEqual::operator());
		//while (it != orderedHull.end())
		//{
		//	std::cout << ""
		//	++it;
		//}
		polygons[j++] = std::make_pair(orderedConvexHull, orderedHull);
	}
	std::cout << "Polygons Ordered" << std::endl;
}


sf::Vector2f MapTools::getNextDirection(Vertex vertex)
{

	typedef sf::Vector2f Vector;
	Vector left = Vector(-1, 0);
	Vector up = Vector(0, -1);
	Vector right = Vector(1, 0);
	Vector down = Vector(0, 1);
	std::array<Vector, 4> convexDirections = { left, up, right, down };
	std::array<Vector, 4> concaveDirections = { up, right, down, left };
	if (std::get<1>(vertex) == VertexType::CONVEX)
	{
		return convexDirections[std::get<2>(vertex)];
	}
	else
	{
		return concaveDirections[std::get<2>(vertex)];
	}
}

Vertex MapTools::makeVertex(Vector vec, VertexType type, int orientation, int label, bool& makeExtendendPoint)
{
	float alpha = GAME_TILESIZE * 0.5f;
	Vector SE = Vector(alpha, alpha);
	Vector SW = Vector(-alpha, alpha);
	Vector NW = Vector(-alpha, -alpha);
	Vector NE = Vector(alpha, -alpha);
	std::array<Vector, 4> convexOrientation = { SW, NW, NE, SE};
	std::array<Vector, 4> concaveOrientation = { NW, NE, SE, SW };
	Vector test1;
	Vector test2;
	int offset;
	if (type == VertexType::CONVEX)
	{
		test1 = vec + convexOrientation[orientation];
		test2 = vec + convexOrientation[(orientation + 1) % 4];
	}
	else if(type == VertexType::CONCAVE)
	{
		test1 = vec + concaveOrientation[orientation];
		test2 = vec + concaveOrientation[(orientation + 1) % 4];
	}
	int label1 = map->getCurrentTile(test1)->region[0];
	int label2 = map->getCurrentTile(test2)->region[0];

	if (label2 == 0)
	{
		if (label1 == 0 && label2 == 0) makeExtendendPoint = true;
		if (type == VertexType::CONVEX) {
			offset = 1;
		}
		else {
			offset = 2;
		}
		int o = (orientation + offset) % 4;
		map->debugTags.push_back(std::to_string(map->debugTags.size()) + "_X" + std::to_string(o));
		return std::make_tuple(vec, VertexType::CONVEX, (orientation + offset) % 4);
	}
	else if (label1 == label && label2 == label)
	{
		if (type == VertexType::CONCAVE) {
			offset = 3;
		}
		else {
			offset = 2;
		}
		int o = (orientation + offset) % 4;
		map->debugTags.push_back(std::to_string(map->debugTags.size()) + "_A" + std::to_string(o));
		return std::make_tuple(vec, VertexType::CONCAVE, (orientation + offset)%4);
	}
	else if (label2 == label && label1 == 0)
	{
		return std::make_tuple(sf::Vector2f(0,0), VertexType::EDGE, 0);
	}
	
}

void MapTools::createNodes()
{
	//nodes.emplace_back(Node());
	std::vector<std::vector<sf::Vector2f>> culledPolygons;
	for (auto& polygon : polygons)
	{
		//culledPolygons.insert(culledPolygons.end(), polygon.first.begin(), polygon.first.end());
		for (auto& vertex : polygon.first)
		{
			//nodes.emplace_back(Node(vertex));
			//Node vertexNode(vertex);
			//size_t vertexHash = vectorHash(vertexNode.position);
			nodes.emplace(vertex, Node(vertex));
		}
	}
	//nodes.emplace_back(Node());

	//for (int j = 0; j < nodes.size() - 1; ++j)
	//{
	for (auto& node1 : nodes)
	{
		//for (int i = 0; i < nodes.size() - 1; ++i)
		for (auto& node2 : nodes)
		{
			if (V2Tools::inLineOfSight_againstPolygon(polygons, node2.first, node1.first))
			{
				float distance = V2Tools::distance(node2.first, node1.first);
				if (distance > 0.1f)
				{
					//nodes[node2.first.hash];
					//nodes.at(node2.first.hash);
					node2.second.neighborHashes.emplace_back(node1.first);
					//.emplace_back(node1.first.hash);
					node2.second.distance.emplace_back(distance);
				}
			}

			//if (V2Tools::inLineOfSight(culledPolygons, node2.first, node1.first))
			//{

			//}
		}
	}
}












#include "stdafx.h"
#include "Map.h"
#include "MapTools.h"
#include "Node.h"


#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <cmath>


Map::~Map()
{
}

void Map::loadFromDiskEditor(const std::string& filename, std::map<std::string, Tile>& tileAtlas) {
	std::ifstream inputFile;
	inputFile.open(filename, std::ios::in | std::ios::binary);

	inputFile.read((char*)&width, sizeof(unsigned int));
	inputFile.read((char*)&height, sizeof(unsigned int));

	for (int i = 0; i < this->width * this->height; ++i) {

		TileType tileType;
		inputFile.read((char*)&tileType, sizeof(int));

		switch (tileType)
		{
		default:
		case TileType::VOID:
		case TileType::CONCRETE:
			this->tiles.emplace_back(tileAtlas.at("Concrete"));
			break;
		case TileType::WALL:
			this->tiles.emplace_back(tileAtlas.at("Wall"));
			break;
		case TileType::WATER:
			this->tiles.emplace_back(tileAtlas.at("Water"));
			break;

		}
		Tile& tile = this->tiles.back();
		inputFile.read((char*)&tile.tileOrientation, sizeof(int));
		inputFile.read((char*)&tile.region, sizeof(int) * 1);
	}
	for (auto& l : this->grid.L) {
		inputFile.read((char*)&l, sizeof(unsigned int));
	}
	for (auto& c : this->grid.C) {
		inputFile.read((char*)&c, sizeof(unsigned int));
	}
	inputFile.close();
}

void Map::loadFromDiskPlaying(const std::string& filename, std::map<std::string, Tile>& tileAtlas) {

	std::ifstream inputFile;
	inputFile.open(filename, std::ios::in | std::ios::binary);

	inputFile.read((char*)&width, sizeof(unsigned int));
	inputFile.read((char*)&height, sizeof(unsigned int));

	for (int i = 0; i < this->width * this->height; ++i) {

		TileType tileType;
		inputFile.read((char*)&tileType, sizeof(int));
		
		switch (tileType)
		{
		default:
		case TileType::VOID:
		case TileType::CONCRETE:
		{
			this->tiles.emplace_back(tileAtlas.at("Concrete"));
			this->tiles[i].initialize();
			break;
		}
			
		case TileType::WALL:
		{
			this->tiles.emplace_back(tileAtlas.at("Wall"));
			this->tiles[i].initialize();
			break;
		}
			
		case TileType::WATER:
		{
			this->tiles.emplace_back(tileAtlas.at("Water"));
			this->tiles[i].initialize();
			break;
		}
			

		}
		Tile& tile = this->tiles.back();
		inputFile.read((char*)&tile.tileOrientation, sizeof(int));
		inputFile.read((char*)&tile.region, sizeof(int)*1);
	}

	this->readMapHitboxesFromStream(inputFile);
	this->grid.readGridFromStream(inputFile);
	this->readPolygonsFromStream(inputFile);
	this->readNodesFromStream(inputFile);
	
	this->constructPlayingData();

	inputFile.close();
}

void Map::saveToDisk(const std::string& filename) {
	this->constructSaveData();
	
	std::ofstream outputFile;
	outputFile.open(filename, std::ios::out | std::ios::binary);

	//MAPSIZE
	outputFile.write((char*)&width, sizeof(unsigned int));
	outputFile.write((char*)&height, sizeof(unsigned int));

	//TILES
	for (auto& tile : this->tiles) {
		outputFile.write((char*)&tile.tileType, sizeof(int));
		outputFile.write((char*)&tile.tileOrientation, sizeof(int));
		outputFile.write((char*)&tile.region, sizeof(int)*1);
	}

	this->writeMapHitboxesToStream(outputFile);
	this->grid.writeGridToStream(outputFile);
	this->writePolygonsToStream(outputFile);
	this->writeNodesToStream(outputFile);

	outputFile.close();
}

void Map::constructSaveData()
{
	// construct maphitboxes, a grid to filter them and polyons for pathfinding
	MapTools mapTools(this);
	mapTools.constructMapObjects(findConnectedRegions({ TileType::WALL, TileType::WATER }, 0));

	//hitboxes,grid,polygons
	this->polygons = mapTools.getPolygons();
	this->hitboxes = mapTools.getHitboxes();
	this->nodes = mapTools.getNodes();
	this->grid = UniformGrid<MapHitbox>(this->width, this->height, &this->hitboxes);

	//pathfinder = PathFinder(&nodes, &polygons);
}

void Map::constructPlayingData()
{
	//polygons to nodes (in pathfinder)
	pathfinder = PathFinder(&nodes,&polygons);
}




void Map::draw(sf::RenderWindow& window, float dt) {
	for (int y = 0; y < this->height; ++y) {
		for (int x = 0; x < this->width; ++x) {
			sf::Vector2f pos;
			pos.x = x * this->tileSize;
			pos.y = y * this->tileSize;
			this->tiles[y*this->width + x].sprite.setPosition(pos);
			this->tiles[y*this->width + x].update(dt);
			this->tiles[y*this->width + x].draw(window);
		}
	}

	std::vector<sf::Color> color = { sf::Color::Red, sf::Color::Blue, sf::Color::Yellow };


	if (this->game->switches.showHitboxHighlights)
	{
		for (std::unordered_set<unsigned int>::const_iterator it = set.begin(); it != set.end(); ++it)
		{
			hitboxes[*it].draw(window);
		}
	}

	if (this->game->switches.showPathfinder)
	{
		if (nodes.size()) {
			for (auto& node : nodes)
			{
				{
					sf::CircleShape drawPoly(6);
					drawPoly.setOrigin(sf::Vector2f(6, 6));
					drawPoly.setPosition(node.first);
					drawPoly.setFillColor(color[0]);
					window.draw(drawPoly);
				}
				{
					for (int i = 0; i < node.second.neighborHashes.size(); ++i)
					{
						sf::Vector2f tmp = node.second.neighborHashes[i] - node.first;
						float length = V2Tools::length(tmp);
						float angle = atan2(tmp.y, tmp.x) * 180 / M_PI + 180;
						tmp = sf::Vector2f(length, 2);
						sf::RectangleShape vector(tmp);
						vector.setPosition(node.second.neighborHashes[i]);
						vector.setRotation(angle);
						vector.setFillColor(color[2]);
						window.draw(vector);
					}
				}
			}
		}
	}
	
	
	if (this->game->switches.showPolygon)
	{
		for (auto& polygon : polygons)
		{
			int k = 0;
			for (auto& vertex : polygon.second)
			{
				//sf::Text text(std::to_string(k++), this->game->fonts.at("main_font"),12);

				sf::CircleShape drawPoly(4);
				drawPoly.setOrigin(sf::Vector2f(4, 4));
				drawPoly.setPosition(vertex);
				drawPoly.setFillColor(color[1]);
				window.draw(drawPoly);

				if (k < debugTags.size())
				{
					sf::Text text(std::string(debugTags[k++]), this->game->fonts.at("main_font"), 12);
					text.setPosition(vertex);
					window.draw(text);
				}
				
			}
		}
	}

	this->set.clear();
}

void Map::setDrawableHitboxes(std::unordered_set<unsigned int>& set)
{
	//this->set = set;
	this->set.insert(set.begin(), set.end());
}

Tile* Map::getCurrentTile(sf::Vector2f pos)
{
	int x = std::floor(pos.x / this->tileSize);
	int y = std::floor(pos.y / this->tileSize);
	if (x >= 0 && y >= 0 && x < this->width && y < this->height)
		return &tiles[y*this->width + x];
	else
		return &this->game->tileAtlas.at("Void");
		//return nullptr;
}




void Map::updateDirection(TileType tileType) {

	for (int y = 0; y < this->height; ++y) {
		for (int x = 0; x < this->width; ++x) {

			int pos = y*this->width + x;

			if (this->tiles[pos].tileType != tileType) continue;

			bool adjacentTiles[3][3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } };

			/* Check for adjacent tiles of the same type */
			if (x > 0 && y > 0)
				adjacentTiles[0][0] = (this->tiles[(y - 1)*this->width + (x - 1)].tileType == tileType);
			if (y > 0)
				adjacentTiles[0][1] = (this->tiles[(y - 1)*this->width + (x)].tileType == tileType);
			if (x < this->width - 1 && y > 0)
				adjacentTiles[0][2] = (this->tiles[(y - 1)*this->width + (x + 1)].tileType == tileType);
			if (x > 0)
				adjacentTiles[1][0] = (this->tiles[(y)*this->width + (x - 1)].tileType == tileType);
			if (x < width - 1)
				adjacentTiles[1][2] = (this->tiles[(y)*this->width + (x + 1)].tileType == tileType);
			if (x > 0 && y < this->height - 1)
				adjacentTiles[2][0] = (this->tiles[(y + 1)*this->width + (x - 1)].tileType == tileType);
			if (y < this->height - 1)
				adjacentTiles[2][1] = (this->tiles[(y + 1)*this->width + (x)].tileType == tileType);
			if (x < this->width - 1 && y < this->height - 1)
				adjacentTiles[2][2] = (this->tiles[(y + 1)*this->width + (x + 1)].tileType == tileType);

			/* Change the tile variant depending on the tile position */
			if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
				this->tiles[pos].tileOrientation = 2;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
				this->tiles[pos].tileOrientation = 7;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
				this->tiles[pos].tileOrientation = 8;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
				this->tiles[pos].tileOrientation = 9;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
				this->tiles[pos].tileOrientation = 10;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2])
				this->tiles[pos].tileOrientation = 0;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1])
				this->tiles[pos].tileOrientation = 1;
			else if (adjacentTiles[2][1] && adjacentTiles[1][0])
				this->tiles[pos].tileOrientation = 3;
			else if (adjacentTiles[0][1] && adjacentTiles[1][2])
				this->tiles[pos].tileOrientation = 4;
			else if (adjacentTiles[1][0] && adjacentTiles[0][1])
				this->tiles[pos].tileOrientation = 5;
			else if (adjacentTiles[2][1] && adjacentTiles[1][2])
				this->tiles[pos].tileOrientation = 6;
			else if (adjacentTiles[1][0])
				this->tiles[pos].tileOrientation = 0;
			else if (adjacentTiles[1][2])
				this->tiles[pos].tileOrientation = 0;
			else if (adjacentTiles[0][1])
				this->tiles[pos].tileOrientation = 1;
			else if (adjacentTiles[2][1])
				this->tiles[pos].tileOrientation = 1;
		}
	}
}

void Map::depthFirstSearch(std::vector<std::vector<sf::Vector2i>>& regCoord, std::vector<TileType>& whitelist, sf::Vector2i pos, int label,  int regionType = 0) {
	if (pos.x < 0 || pos.x >= this->width) return;
	if (pos.y < 0 || pos.y >= this->height) return;
	if (this->tiles[pos.y * this->width + pos.x].region[regionType] != 0) return;

	std::array<sf::Vector2i, 4> directions = { pos + sf::Vector2i(-1, 0) ,pos + sf::Vector2i(0, 1) ,pos + sf::Vector2i(1, 0),pos + sf::Vector2i(0, -1) };

	
	bool found = false;
	//bool edge = false;
	for (auto type : whitelist)
	{
		if (type == this->tiles[pos.y * this->width + pos.x].tileType)
		{
			found = true;
			//for (auto& d : directions)
			//{
			//	if (d.x < 0 || d.x >= this->width) edge = true;
			//	if (d.y < 0 || d.y >= this->height) edge = true;
			//	if (this->tiles[d.y * this->width + d.x].tileType != type)
			//	{
			//		edge = true;
			//		break;
			//	}
			//		
			//}
			break;
		}
	}
	if (!found) return;

	this->tiles[pos.y* this->width + pos.x].region[regionType] = label;
	//if(edge)
		regCoord[label-1].push_back(pos);

	depthFirstSearch(regCoord, whitelist, directions[0], label, regionType );
	depthFirstSearch(regCoord, whitelist, directions[1], label, regionType );
	depthFirstSearch(regCoord, whitelist, directions[2], label, regionType );
	depthFirstSearch(regCoord, whitelist, directions[3], label, regionType );
}

std::vector<std::vector<sf::Vector2i>> Map::findConnectedRegions(std::vector<TileType> whitelist, int regionType = 0) {
	std::vector<std::vector<sf::Vector2i>> regCoord;
	int region = 1;

	for (auto& tile : this->tiles) tile.region[regionType] = 0;

	for (int y = 0; y < this->height; ++y) {
		for (int x = 0; x < this->width; ++x) {
			
			bool found = false;
			for (auto type : whitelist)
			{
				if (type == this->tiles[y * this->width + x].tileType)
				{
					found = true;
					break;
				}
			}
			if (this->tiles[y * this->width + x].region[regionType] == 0 && found)
			{
				regCoord.resize(region);
				depthFirstSearch(regCoord, whitelist, sf::Vector2i(x, y), region++, regionType);
			}
		}
	}
	this->numRegions[regionType] = region-1;
	return regCoord;
}

void Map::writePolygonToStream(std::vector<sf::Vector2f>& polygon, std::ofstream& outputFile)
{
	unsigned int size = polygon.size();
	outputFile.write((char*)&size, sizeof(unsigned int));
	for (auto& vertex : polygon)
	{
		outputFile.write((char*)&vertex.x, sizeof(float));
		outputFile.write((char*)&vertex.y, sizeof(float));
	}


	//unsigned int debugTagSize = debugTags.size();
	//outputFile.write((char*)&debugTagSize, sizeof(unsigned int));
	//for (auto& tag : debugTags)
	//{
	//	unsigned int tagSize = tag.size();
	//	outputFile.write((char*)&tagSize, sizeof(unsigned int));
	//	outputFile.write(tag.c_str(),tagSize);
	//	//outputFile.write((char*)&tag, 3 * sizeof(char));

	//}
}

void Map::readPolygonFromStream(std::vector<sf::Vector2f>& polygon, std::ifstream& inputFile)
{
	unsigned int size;
	inputFile.read((char*)&size, sizeof(unsigned int));
	polygon.resize(size);
	for (auto& vertex : polygon)
	{
		inputFile.read((char*)&vertex.x, sizeof(float));
		inputFile.read((char*)&vertex.y, sizeof(float));
	}


	//unsigned int debugTagSize;
	//inputFile.read((char*)&debugTagSize, sizeof(unsigned int));
	//debugTags.resize(debugTagSize);
	//for (auto& tag : debugTags)
	//{
	//	unsigned int tagSize;
	//	inputFile.read((char*)&tagSize, sizeof(unsigned int));
	//	char* temp = new char[tagSize + 1];
	//	inputFile.read(temp, tagSize);
	//	temp[tagSize] = '\0';
	//	tag = temp;
	//	delete[] temp;
	//}
}

void Map::writePolygonsToStream(std::ofstream& outputFile)
{
	unsigned int size = polygons.size();
	outputFile.write((char*)&size, sizeof(unsigned int));
	for (auto& polygon : this->polygons)
	{
		writePolygonToStream(polygon.first, outputFile);
		writePolygonToStream(polygon.second, outputFile);
	}
}

void Map::readPolygonsFromStream(std::ifstream& inputFile)
{
	unsigned int size;
	inputFile.read((char*)&size, sizeof(unsigned int));
	this->polygons.resize(size);
	for (auto& polygon : this->polygons)
	{
		readPolygonFromStream(polygon.first, inputFile);
		readPolygonFromStream(polygon.second, inputFile);
	}
}

void Map::writeMapHitboxesToStream(std::ofstream& outputFile)
{
	unsigned int size = hitboxes.size();
	outputFile.write((char*)&size, sizeof(unsigned int));
	sf::Vector2f hitboxPoint;
	for (auto& hitbox : this->hitboxes)
	{
		// get diagonal size of Hitbox (down right point)
		hitboxPoint = hitbox.getPoint(2);
		outputFile.write((char*)&hitboxPoint.x, sizeof(float));
		outputFile.write((char*)&hitboxPoint.y, sizeof(float));

		//get position of Hitbox
		hitboxPoint = hitbox.getPosition();
		outputFile.write((char*)&hitboxPoint.x, sizeof(float));
		outputFile.write((char*)&hitboxPoint.y, sizeof(float));
	}
}

void Map::readMapHitboxesFromStream(std::ifstream& inputFile)
{
	unsigned int size;
	inputFile.read((char*)&size, sizeof(unsigned int));
	sf::Vector2f hitboxPoint;
	for (int j = 0; j < size; ++j)
	{
		//construct MapHitbox with size
		inputFile.read((char*)&hitboxPoint.x, sizeof(float));
		inputFile.read((char*)&hitboxPoint.y, sizeof(float));
		this->hitboxes.emplace_back(MapHitbox(hitboxPoint));

		// set MapHitbox to position
		inputFile.read((char*)&hitboxPoint.x, sizeof(float));
		inputFile.read((char*)&hitboxPoint.y, sizeof(float));
		hitboxes.back().setPosition(hitboxPoint);
	}
}

void Map::writeNodesToStream(std::ofstream& outputFile)
{
	unsigned int size = nodes.size();
	outputFile.write((char*)&size, sizeof(unsigned int));
	for (auto& node : nodes)
	{
		V2Tools::writeVectorToStream(node.second.position, outputFile);
		size = (unsigned int)node.second.neighborHashes.size();
		outputFile.write((char*)&size, sizeof(unsigned int));
		for (int i = 0; i < node.second.neighborHashes.size(); ++i)
		{
			V2Tools::writeVectorToStream(node.second.neighborHashes[i], outputFile);
			outputFile.write((char*)&node.second.distance[i], sizeof(float));
		}

	}
}

void Map::readNodesFromStream(std::ifstream& inputFile)
{
	unsigned int size;
	inputFile.read((char*)&size, sizeof(unsigned int));
	Node tmp;
	for (int j = 0; j < size; ++j)
	{
		sf::Vector2f position;
		V2Tools::readVectorFromStream(position, inputFile);
		unsigned int neighborSize;
		inputFile.read((char*)&neighborSize, sizeof(unsigned int));

		std::vector<sf::Vector2f> neighbors(neighborSize);
		std::vector<float> distance(neighborSize);
		for (int i = 0; i < neighbors.size(); ++i)
		{
			V2Tools::readVectorFromStream(neighbors[i], inputFile);
			inputFile.read((char*)&distance[i], sizeof(float));
		}
		Node tmp(position);
		tmp.neighborHashes = neighbors;
		tmp.distance = distance;
		nodes.emplace(tmp.position, tmp);
	}
}



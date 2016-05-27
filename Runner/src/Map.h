#pragma once
#include <SFML/Graphics.hpp>

#include <string>
//#include <map>
#include <vector>

#include "Tile.h"
#include "MapHitbox.h"
#include "UniformGrid.h"
#include "GameSettingConstants.h"
#include "AStarSearch.h"

class Map
{
public:
	Map(Game* game) : tileSize(GAME_TILESIZE), width(0), height(0) { 
		numRegions[0] = 0;
		this->game = game;
	};
	//Map(unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas) : tileSize(GAME_TILESIZE), width(width), height(height) {
	//	for (int i = 0; i < width * height; ++i) {
	//		this->tiles.push_back(tileAtlas.at("Grass"));
	//	}
	//};
	~Map();

	std::vector<std::string> debugTags;

	Game* game;

	unsigned int width;
	unsigned int height;

	std::vector<Tile> tiles;
	
	std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>> polygons;
	std::vector<MapHitbox> hitboxes;
	UniformGrid<MapHitbox> grid;
	PathFinder pathfinder;
	NodeHashMap nodes;


	unsigned int tileSize;
	unsigned int numSelected;
	unsigned int numRegions[1];

	void loadFromDiskEditor(const std::string& filename, std::map<std::string, Tile>& tileAtlas);
	void loadFromDiskPlaying(const std::string& filename, std::map<std::string, Tile>& tileAtlas);
	void saveToDisk(const std::string& filename);

	void draw(sf::RenderWindow& window, float dt);

	Tile* getCurrentTile(sf::Vector2f);

	void constructPlayingData();
	void constructSaveData();

	//plot activated hitboxes
	void setDrawableHitboxes(std::unordered_set<unsigned int>& set);
	std::unordered_set<unsigned int> set;

private:

	void writeMapHitboxesToStream(std::ofstream& outputFile);
	void readMapHitboxesFromStream(std::ifstream& inputFile);

	void writePolygonToStream(std::vector<sf::Vector2f>& polygon, std::ofstream& outputFile);
	void readPolygonFromStream(std::vector<sf::Vector2f>& polygon, std::ifstream& inputFile);

	void writePolygonsToStream(std::ofstream& outputFile);
	void readPolygonsFromStream(std::ifstream& inputFile);

	void writeNodesToStream(std::ofstream&);
	void readNodesFromStream(std::ifstream&);

	std::vector<std::vector<sf::Vector2i>> findConnectedRegions(std::vector<TileType> whiteList, int type);
	
	void updateDirection(TileType tileType);

	//bool isEdgeTile(sf::Vector2i pos, int regionTyp);
	void depthFirstSearch(std::vector<std::vector<sf::Vector2i>>& regCoord, std::vector<TileType>& whiteList, sf::Vector2i pos, int label, int type);
};


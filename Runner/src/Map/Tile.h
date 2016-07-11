#pragma once
#include <SFML/Graphics.hpp>
//#include <vector>
#include "GameEntities/AnimationHandler.h"





enum class TileType {VOID, CONCRETE, WALL, WATER, GRASS};

std::string tileTypeToStr(TileType type);

class Tile 
	//: public IAtomicEntity
{
public:
	Tile() { };
	Tile(const unsigned int height, const unsigned int width, sf::Texture& texture, const Animation& animation, const TileType tileType, const unsigned int numberOfVariants);
	Tile(const unsigned int height, const unsigned int width, const TileType tileType);
	//Tile(const Tile&);
	~Tile();

	AnimationHandler animHandler;
	sf::Sprite sprite;

	TileType tileType;

	unsigned int numberOfVariants;

	unsigned int tileOrientation;

	unsigned int region[1];

	void draw(sf::RenderWindow& window);
	void update(float dt);
	void initialize();

};


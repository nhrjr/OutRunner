#pragma once
#include <SFML/Graphics.hpp>
//#include <vector>
#include "AnimationHandler.h"





enum class TileType {VOID, CONCRETE, WALL, WATER, GRASS};

std::string tileTypeToStr(TileType type);

class Tile 
	//: public IGameEntity
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
	//virtual void setPosition(sf::Vector2f position);
	//virtual sf::Vector2f getPosition();

	//virtual sf::Vector2f getPoint(int i);
	//virtual int getPointCount();
	//virtual sf::Transform getTransform();
	//virtual float getMinDistance();
};


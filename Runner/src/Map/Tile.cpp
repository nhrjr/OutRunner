#include "stdafx.h"

#include "Tile.h"

#include <iostream>



Tile::Tile(const unsigned int height, const unsigned int width, sf::Texture& texture, const Animation& animation, const TileType tileType, const unsigned int numberOfVariants) :
	tileType(tileType), tileOrientation(0), numberOfVariants(numberOfVariants) {
	region[0] = 0;
	sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));

	sprite.setTexture(texture);
	//animHandler.frameSize = sf::IntRect(0, 0, width, height);
	//animHandler.bounds = animHandler.frameSize;
	animHandler.addAnim(animation);
	//animHandler.setRandomAnimVariant(numberOfVariants);
	
	//animHandler.update(0.0f);
}

Tile::Tile(const unsigned int height, const unsigned int width, const TileType tileType) :
	tileType(tileType), tileOrientation(0), numberOfVariants(0) {
	region[0] = 0;
}


//Tile::Tile(const Tile& tile) : animHandler(tile.animHandler), sprite(tile.sprite), tileType(tile.tileType), tileVariant(tile.tileVariant), tileOrientation(tile.tileOrientation)
//{
//	region[0] = tile.region[0];
//	animHandler.setRandomStart();
//}

Tile::~Tile() {
};

void Tile::initialize()
{
	//animHandler.setRandomAnimVariant(numberOfVariants);
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Tile::update(float dt)
{
	//animHandler.changeAnim(tileOrientation);
	//animHandler.setAnim(numberOfVariants);
	animHandler.update(dt);
	sprite.setTextureRect(animHandler.bounds);
}

std::string tileTypeToStr(TileType type) {

	switch (type)
	{
	default:
	case TileType::VOID:
		return "Void";
	case TileType::CONCRETE:
		return "Concrete";
	case TileType::WALL:
		return "Wall";
	case TileType::WATER:
		return "Water";
	case TileType::GRASS:
		return "Grass";
	}

}

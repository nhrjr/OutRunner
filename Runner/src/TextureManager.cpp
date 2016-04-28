#include "stdafx.h"
#include "TextureManager.h"



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

void TextureManager::loadTexture(const std::string& name, const std::string& filename) {
	sf::Texture tex;
	tex.loadFromFile(filename);

	this->textures[name] = tex;
}

sf::Texture& TextureManager::getRef(const std::string& texture) {
	return this->textures.at(texture);
}

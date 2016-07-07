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
	//tex.setSmooth(true);

	this->textures[name] = tex;
}

sf::Texture& TextureManager::getRef(const std::string& texture) {
	auto it = this->textures.find(texture);
	// TODO: think about crashing, or not? when texture is not found.
	//if (it != this->textures.end())
	//	return it->second;
	//else
	//	return sf::Texture();
	return it->second;
}

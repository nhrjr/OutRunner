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
	if (it != this->textures.end())
		return it->second;
	else
		return sf::Texture();
}

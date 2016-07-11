#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void loadTexture(const std::string& name, const std::string& filename);

	sf::Texture& getRef(const std::string& texture);

private:
	std::unordered_map<std::string, sf::Texture> textures;
};


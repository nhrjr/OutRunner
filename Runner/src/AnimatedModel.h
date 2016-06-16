#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationHandler.h"
#include "TextureManager.h"

class SpriteDefinition {
public:
	SpriteDefinition(const std::unordered_map<int, Animation>& ss, sf::Vector2f p, float s, sf::Texture& t ) : pos(p), scale(s), states(ss), texture(t)
	{
		handler.addAnim(states.begin()->second);
	}
	AnimationHandler handler;
	std::unordered_map<int, Animation> states;
	sf::Vector2f pos;
	float scale;

	sf::Texture& texture;

	bool newState = false;
	int state;
	int nextState;

	void setnextState(const int& news)
	{
		nextState = news;
		if (nextState != state)
			newState = true;

	}

	void update(float dt) {

		if (newState)
		{
			for (auto& s : states)
			{
				if (nextState == s.first)
				{
					handler.addAnim(states[nextState]);
					state = s.first;
				}
			}
			newState = false;
		}
		handler.update(dt);
	}
};

class AnimatedModel : public sf::Drawable, public sf::Transformable
{
public:


	std::vector<sf::Sprite> sprites;
	std::vector<SpriteDefinition> spriteDefs;

	~AnimatedModel() = default;

	template<class... Ts>
	AnimatedModel(Ts... defs)
	{
		addSprite(defs...);
	};
	

	template<class T>
	void addSprite(const T& def) {
		sprites.emplace_back(sf::Sprite());
		sprites.back().setTexture(def.texture);
		sprites.back().setOrigin(def.pos.x, def.pos.y);
		sprites.back().setScale(def.scale, def.scale);
		spriteDefs.emplace_back(def);
	}

	template<class T, class... Ts>
	void addSprite(T def, Ts... defs) {
		addSprite(def);
		addSprite(defs...);
	}

	void update(float dt) {

		for (auto& d : spriteDefs)
		{
			d.update(dt);			
		}
		for (int i = 0; i < sprites.size(); ++i)
		{
			sprites[i].setTextureRect(spriteDefs[i].handler.bounds);
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		for (auto& sprite : sprites)
		{
			target.draw(sprite, states);
		}
	}


};

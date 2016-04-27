#pragma once

#include <SFML/Graphics.hpp>
#include <vector>





class Animation
{
public:
	Animation() : startFrame(0), endFrame(0), frameDuration(0) {};
	Animation(unsigned int startFrame, unsigned int endFrame, float duration) : startFrame(startFrame), endFrame(endFrame), frameDuration(duration) {};
	~Animation();

	unsigned int startFrame;
	unsigned int endFrame;

	float frameDuration;

	unsigned int getLength() { return endFrame - startFrame + 1; }


};


class AnimationHandler {
public:
	AnimationHandler() : t(0.0f), currentAnim(-1) {};
	AnimationHandler(const sf::IntRect& frameSize) : frameSize(frameSize), t(0.0f), currentAnim(-1) {
	};

	void addAnim(const Animation& anim);

	void update(const float dt);

	void changeOrientation(unsigned int tileOrientation, unsigned int numberOfVariants);

	void setAnimVariant(unsigned int numberOfVariants);

	void setRandomStart();

	sf::IntRect bounds;

	sf::IntRect frameSize;

private:
	Animation animation;

	float t;

	int currentAnim;



};

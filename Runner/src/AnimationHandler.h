#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "signal_slot.h"





class Animation
{
public:
	Animation() : startFrame(0), endFrame(0), frameDuration(0) {};
	Animation(unsigned int startFrame, unsigned int endFrame, float duration, int width, int height, bool randomOrNot, int anim) : startFrame(startFrame), endFrame(endFrame), frameDuration(duration),
		width(width), height(height), random(randomOrNot), currentAnim(anim) {
	};
	~Animation();

	unsigned int startFrame;
	unsigned int endFrame;

	float frameDuration;
	int currentAnim;
	int width;
	int height;
	bool random;


	unsigned int getLength() { return endFrame - startFrame + 1; }


};


class AnimationHandler {
public:
	AnimationHandler() : t(0.0f), currentAnim(-1) {};
	//AnimationHandler(const sf::IntRect& frameSize) : frameSize(frameSize), t(0.0f), currentAnim(-1) {
	//};

	void addAnim(const Animation& anim);

	bool update(const float dt);

	void changeOrientation(unsigned int tileOrientation, unsigned int numberOfVariants);

	void setRandomAnimVariant(unsigned int numberOfVariants);

	void setCurrentAnim(unsigned int current);

	void setRandomStart();

	sf::IntRect bounds;

	sf::IntRect frameSize;


	Animation animation;

	float t;

private:

	int currentAnim;

	



};

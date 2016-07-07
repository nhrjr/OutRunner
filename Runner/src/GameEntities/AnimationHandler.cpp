#include "stdafx.h"
#include "AnimationHandler.h"

#include <random>
#include <iostream>

typedef std::mt19937	                             Engine;
typedef std::uniform_real_distribution<float>        FloatDistribution;
typedef std::uniform_int_distribution<>              IntDistribution;

Animation::~Animation()
{
}

bool AnimationHandler::update(const float dt) {
	if (currentAnim < 0) return false;

	float duration = this->animation.frameDuration;

	if (duration == 0.0f) return false;

	int frame = int((t + dt) / duration);

	frame %= this->animation.getLength();

	sf::IntRect rect = this->frameSize;
	rect.left = rect.width * frame;
	rect.top = rect.height * currentAnim;

	this->bounds = rect;

	this->t += dt;

	if (this->t > duration * this->animation.getLength()) {
		this->t = 0.0f;
		return true;
	}
	return false;
}

void AnimationHandler::addAnim(const Animation& anim) {
	this->animation = anim;
	this->frameSize = sf::IntRect(0, 0, animation.width, animation.height);
	if (animation.random)
	{
		setRandomAnimVariant(animation.currentAnim);
	}
	else
	{
		this->currentAnim = animation.currentAnim;
		sf::IntRect rect = this->frameSize;
		rect.left = 0;
		rect.top = rect.height * currentAnim;
		this->bounds = rect;
	}
}

void AnimationHandler::setRandomAnimVariant(unsigned int numberOfVariants)
{
	if(numberOfVariants > 1)
	{
		std::random_device rd;
		Engine eng(rd());
		IntDistribution dist(0, numberOfVariants-1);
		this->currentAnim = dist(eng);
		sf::IntRect rect = this->frameSize;
		rect.left = 0;
		rect.top = rect.height * currentAnim;
		this->bounds = rect;
	}
	//this->currentAnim = 0;
	if (animation.getLength() > 1)
	{
		this->setRandomStart();
	}
}

void AnimationHandler::setCurrentAnim(unsigned int current)
{
	this->currentAnim = current;
}

void AnimationHandler::changeOrientation(unsigned int tileOrientation, unsigned int numberOfVariants) {
	if (currentAnim < 0) return;
	setRandomAnimVariant(numberOfVariants);
	this->currentAnim += tileOrientation * numberOfVariants;
}

void AnimationHandler::setRandomStart()
{
	if (currentAnim >= 0)
	{
		std::random_device rd;
		Engine eng(rd());
		FloatDistribution dist(0, animation.getLength() * animation.frameDuration);
		this->t = dist(eng);
	}
}

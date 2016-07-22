#pragma once

#include "cocos2d.h"

static const char s_stars1[] = "Images/stars.png";
static const char s_back3[] = "Images/background3.png";

class ParticleSprite : public cocos2d::Sprite
{
public:
	ParticleSprite();
	~ParticleSprite();

	void toggleCallback();
    inline cocos2d::ParticleSystemQuad* getEmitter(){return emitter_;}

protected:
	cocos2d::ParticleSystemQuad* emitter_;
};

class HellFire : public ParticleSprite
{
public:
	HellFire();

	virtual void onEnter() override;
    
    void initHellFire();
};

class Sonic : public ParticleSprite
{
public:
	Sonic();

	CREATE_FUNC(Sonic);
	virtual void onEnter() override;

};

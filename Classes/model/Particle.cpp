#include "Particle.h"
#include "../layer/GameLayer.h"
#include "../codechiev/BasisUtil.h"

USING_NS_CC;

ParticleSprite::ParticleSprite():emitter_(nullptr){}

ParticleSprite::~ParticleSprite()
{
	if (emitter_)
	{
		emitter_->release();
	}
}

void ParticleSprite::toggleCallback()
{
	if (emitter_ != nullptr)
	{
		if (emitter_->getPositionType() == ParticleSystem::PositionType::GROUPED)
			emitter_->setPositionType(ParticleSystem::PositionType::FREE);
		else if (emitter_->getPositionType() == ParticleSystem::PositionType::FREE)
			emitter_->setPositionType(ParticleSystem::PositionType::RELATIVE);
		else if (emitter_->getPositionType() == ParticleSystem::PositionType::RELATIVE)
			emitter_->setPositionType(ParticleSystem::PositionType::GROUPED);
	}
}

void
HellFire::initHellFire()
{
	emitter_ = ParticleSystemQuad::create("Particles/hellfire.plist");
	emitter_->retain();
	addChild(emitter_, 10);
}


Sonic::Sonic()
{
	emitter_ = ParticleSystemQuad::create("Particles/sonic.plist");
	emitter_->retain();
	addChild(emitter_);
}

void
Sonic::onEnter()
{
	ParticleSprite::onEnter();
}
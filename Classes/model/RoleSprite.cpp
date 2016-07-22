#include "RoleSprite.hpp"
#include "ActionSprite.h"
#include "Effect.hpp"
#include "ActionQueue.hpp"
#include "../layer/GameLayer.h"
#include "../codechiev/base/singleton.h"
#include "../codechiev/BasisUtil.h"

USING_NS_CC;
using namespace codechiev;

RoleSprite::RoleSprite():
walkAction(nullptr),
attackAction(nullptr),
idleAction(nullptr),
faceAction(nullptr),
backAction(nullptr){};

RoleSprite::~RoleSprite()
{
    if(walkAction)
        walkAction->release();
    if(attackAction)
        attackAction->release();
    if(idleAction)
        idleAction->release();
    if(faceAction)
        faceAction->release();
    if(backAction)
        backAction->release();
    
    log("~RoleSprite");
}

bool
RoleSprite::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    
    return true;
}

void
RoleSprite::forever(cocos2d::Animate * anim)
{
    if(!anim)
        return;
    this->stopAllActions();
    Action* action = RepeatForever::create(anim);
    this->runAction(action);
}

void
RoleSprite::once(cocos2d::Animate * anim)
{
    if(!anim)
        return;
    this->stopAllActions();
    this->runAction(anim);
}

void
RoleSprite::idle()
{
   forever(idleAction);
}

void
RoleSprite::attack()
{
    once(attackAction);
}

void
RoleSprite::walk()
{
	forever(walkAction);
}


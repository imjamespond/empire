//
//  RoleLayer.cpp
//  empire
//
//  Created by metasoft on 16/5/4.
//
//
#include "ActionSprite.h"
#include "Effect.hpp"
#include "RoleLayer.hpp"
#include "RoleSprite.hpp"
#include "../codechiev/BasisUtil.h"

USING_NS_CC;
using namespace codechiev;

Role::Role():role(nullptr),
aura(nullptr){}
Role::~Role()
{
    log("~Role");
}

bool
Role::initRole()
{
    aura = Effect::create();
    aura->initEffect(gAnimAura, 0);
    aura->setPosition(Vec2::ZERO);
    aura->setScale(5.0f);
    this->addChild(aura);
    
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    assert(ac->roleStructMap.find(id)!=ac->roleStructMap.end());
    struct RoleStruct &st = ac->roleStructMap[id];
    
    role = RoleSprite::create();
    role->walkAction = getAction(st.walk);
    role->attackAction = getAction(st.attack);
    role->idleAction = getAction(st.idle);
    role->faceAction = getAction(st.face);
    role->backAction = getAction(st.back);
    role->idle();
    this->addChild(role);

    this->addChild(Label::createWithSystemFont(name, "arial", 20));
    
    return true;
}

void
Role::setPetrify(bool petr)
{
    if(petr)
        role->setColor(cocos2d::Color3B::GRAY);
    else
        role->setColor(cocos2d::Color3B(0xff,0xff,0xff));
}


float
Role::walkTo(const Vec2& to, float speed)
{
    if(to.y<this->getPosition().y){
        role->forever(role->faceAction);
    }else{
        role->forever(role->backAction);
    }
    
    Vec2 dist = this->getPosition() - to;
    float duration = dist.length() / speed;
    auto cb = CallFunc::create(std::bind([=](RoleSprite* r) {
        r->idle();
    }, role));
    this->runAction(Sequence::create( MoveTo::create(duration, to), cb, nullptr));
    return duration;
}
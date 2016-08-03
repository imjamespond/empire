//
//  effect.cpp
//  empire
//
//  Created by metasoft on 16/5/5.
//
//

#include "Effect.hpp"
#include "ActionSprite.h"
//#include "ActionQueue.hpp"
//#include "../codechiev/base/singleton.h"

USING_NS_CC;
using namespace codechiev;

void
Effect::initEffect(const struct AnimationStruct& anim, float delay, bool repeat)
{
    auto frames = getAnimation(anim);
    auto spriteAnim = Animation::createWithSpriteFrames(frames, anim.duration / (anim.end-anim.begin));
    
    if(repeat)
    {
        this->runAction(RepeatForever::create(Animate::create(spriteAnim)));
    }else
    {
        auto cb = CallFunc::create(std::bind([=](Sprite* s){
            s->removeFromParent();
        }, this) );
        auto sequence = Sequence::create(DelayTime::create(delay),
                                         Animate::create(spriteAnim), cb, nullptr);
        this->runAction(sequence);
    }
}
void
Effect::initEffect2(const struct AnimationStruct& anim)
{
    auto spriteAnim = Animation::createWithSpriteFrames(getAnimation(anim), anim.duration / (anim.end-anim.begin));
    auto cb = CallFunc::create(std::bind([=](Sprite* s){
        s->setRotation(0.0f);
    }, this) );
    this->runAction(RepeatForever::create(Sequence::create(Animate::create(spriteAnim), RotateBy::create(.3, 180), cb, nullptr)));
}
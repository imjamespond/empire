//
//  GameAnim.cpp
//  empire
//
//  Created by metasoft on 16/5/20.
//
//

#include "GameAnim.h"
#include "Effect.hpp"
#include "RoleSprite.hpp"
#include "ActionSprite.h"
#include "ActionQueue.hpp"
#include "ScreenShake.hpp"
#include "../codechiev/BasisUtil.h"
#include "../layer/GameSceneLayer.h"
#include "../layer/GameMenuLayer.hpp"
#include "../layer/GameLayer.h"
#include "../layer/NotificationLayer.h"

USING_NS_CC;
using namespace codechiev;

void
YanWangAnim::play()
{
    /*Effect *explosion = Effect::create();
    explosion->setPosition(target->getPosition());
    explosion->initEffect(gAnimExplode, 1, false);
    scene->addChild(explosion);*/
    
    role->role->stopAllActions();
    role->role->runAction(Sequence::create(role->role->attackAction,
                                           CallFunc::create(std::bind([=](RoleSprite* role,
                                                                          Game::Player* enemy,
                                                                          int hp, int maxhp){
        base::Singleton<ActionQueue>::get()->playNext();
        role->idle();
        
        GameMenuLayer *gm = gGameLayer->gameMenu;
        gm->updateHealth(*enemy, hp, maxhp);
    }, role->role, enemy, hp, enemy->maxHp) ), nullptr));
    
    scene->sonic->getEmitter()->resetSystem();
    scene->sonic->setPosition(role->getPosition());
    scene->sonic->runAction(Sequence::create(MoveTo::create(1, target->getPosition()),
                                      CallFunc::create(std::bind([=](Sonic* s) {
        s->getEmitter()->stopSystem();
        gGameLayer->runAction(ScreenShake::create(.5, 5));
    }, scene->sonic)), nullptr));
    
    gNotificationLayer->onError(StringUtils::format("阎王 伤害: %d",hurt));
}
void
WuChangAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("无常 延长: %d",prolong));
    
    role->role->stopAllActions();
    role->role->runAction(Sequence::create(role->role->attackAction,
                                           CallFunc::create(std::bind([=](RoleSprite* role){
        base::Singleton<ActionQueue>::get()->playNext();
        role->idle();
    }, role->role) ), cb,nullptr));
    
    self->prolong+=prolong;
}
void
MengPoAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->onError(StringUtils::format("孟婆 恢复: %d",heal));
    
    role->role->stopAllActions();
    role->role->runAction(Sequence::create(role->role->attackAction,
                                           CallFunc::create(std::bind([=](RoleSprite* role,
                                                                          Game::Player* self,
                                                                          int hp, int maxhp){
        base::Singleton<ActionQueue>::get()->playNext();
        role->idle();
        
        GameMenuLayer *gm = gGameLayer->gameMenu;
        gm->updateHealth(*self, hp, maxhp);
    }, role->role, self, hp, self->maxHp) ), cb,nullptr));
}
void
NiuTouAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->onError(StringUtils::format("牛头 缩短: %d",shorten));
    
    role->role->stopAllActions();
    role->role->runAction(Sequence::create(role->role->attackAction,
                                           CallFunc::create(std::bind([=](RoleSprite* role){
        base::Singleton<ActionQueue>::get()->playNext();
        role->idle();
    }, role->role) ), cb, nullptr));
}
void
AnubisAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("AnubisAnim hurt: %d",hurt));
}
void
PharaohAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("PharaohAnim heal: %d", this->heal));
}
void
SphinxAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("SphinxAnim reflect: %d",this->reflect));
}
void
CleopatraAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("CleopatraAnim"));
}
void
LinLuAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("LinLuAnim hurt: %d", hurt));
}
void
YuSheAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("YuSheAnim heal: %d, hurt: %d", this->heal, this->hurt));
}
void
JiShenAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("JiShenAnim heal: %d",this->heal));
}
void
YunShenAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("YunShenAnim"));
}
void
KnightAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("KnightAnim hurt: %d",this->hurt));
}

void
AngelAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](Game::Player* target,
                                             int hp, int maxhp){
        base::Singleton<ActionQueue>::get()->playNext();
        
        GameMenuLayer *gm = gGameLayer->gameMenu;
        gm->updateHealth(*target, hp, maxhp);
    }, self, hp, self->maxHp));
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("AngelAnim heal: %d",this->heal));
}

void
TitanAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](Game::Player* target,
                                             int hp, int maxhp){
        base::Singleton<ActionQueue>::get()->playNext();
        
        GameMenuLayer *gm = gGameLayer->gameMenu;
        gm->updateHealth(*target, hp, maxhp);
    }, enemy, hp, enemy->maxHp));
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("TitanAnim hurt: %d",this->hurt));
}

void
MedusaAnim::play()
{
    auto cb = CallFunc::create(std::bind([=](){
        base::Singleton<ActionQueue>::get()->playNext();
    }) );
    gNotificationLayer->runAction(Sequence::create(DelayTime::create(1), cb, nullptr));
    gNotificationLayer->onError(StringUtils::format("MedusaAnim"));
}



//
//  PvpSearchLayer.cpp
//  empire
//
//  Created by metasoft on 16/6/29.
//
//

#include "PvpSearchLayer.hpp"
#include "HallLayer.hpp"
#include "../layer/MenuLayer.h"
#include <cocostudio/CocoStudio.h>

using namespace cocos2d;
using namespace codechiev;

PvpSearchLayer::PvpSearchLayer(){}

bool
PvpSearchLayer::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kPvpSearchLayer)));
    
    rotateNode = modalLayer->getChildByName("Node_Rotation");
    rotateGlass = rotateNode->getChildByName("Sprite_Glass");
    
    return true;
}

void
PvpSearchLayer::undoModal()
{
    gMenuLayer->pvpSearchLayer = nullptr;
    
    rotateNode->stopAllActions();
    rotateGlass->stopAllActions();
    
    this->unschedule(SEL_SCHEDULE(&PvpSearchLayer::countdownUpdate));
    
    ModalLayer::undoModal();
}

void
PvpSearchLayer::doModal()
{
    auto rot = RotateBy::create(2, 360);
    auto seq = RepeatForever::create(rot);
    
    auto rot_back = rot->reverse();
    auto rot_back_fe = RepeatForever::create(rot_back);
    rotateNode->runAction(seq);
    rotateGlass->runAction(rot_back_fe);
    
    left=5;
    countdownUpdate(0);
    this->schedule(SEL_SCHEDULE(&PvpSearchLayer::countdownUpdate), 1);
    
    ModalLayer::doModal();
}

void
PvpSearchLayer::countdownUpdate(float dt)
{
    if(left>=0){
        auto cd = static_cast<ui::Text*>( modalLayer->getChildByName("Text_CountDown"));
        cd->setString(StringUtils::format("%g", left));
    }
    
    if(left<=0){
        undoModal();
        gMenuLayer->hallLayer->gamePveAlloc();
    }
    
    left--;
}

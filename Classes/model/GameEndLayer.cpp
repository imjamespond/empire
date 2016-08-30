//
//  GameEndLayer.cpp
//  empire
//
//  Created by metasoft on 16/5/5.
//
//

#include "GameEndLayer.hpp"
#include "ActionQueue.hpp"
#include "HallLayer.hpp"
#include "NavigateLayer.hpp"
#include "User.h"
#include "../codechiev/BasisUtil.h"
#include "../layer/GameLayer.h"
#include "../layer/GameMenuLayer.hpp"
#include "../layer/GameSceneLayer.h"
#include "../layer/MenuLayer.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace codechiev;

GameEndLayer::GameEndLayer(){}

bool
GameEndLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    /*Label* label = Label::createWithSystemFont("游戏结束", "", 100);//  //
    this->addChild(label);
    
    label->setPosition(Vec2(gCenter.x, gCenter.y*.5));
    
    auto move = MoveBy::create(1.0f, Vec2(0, gCenter.y*.5f));
    auto move_ease_in = EaseExponentialIn::create(move->clone());
    auto move_ease_out = EaseExponentialOut::create(move->clone());
    label->runAction(Sequence::create(
        move_ease_out,
        DelayTime::create(.5),
        move_ease_in,
        CallFunc::create(std::bind([=](Layer* layer){
    },this)), nullptr));*/
    
    auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(darkLayer);
    
    node = CSLoader::createNode(kGameEndLayer);
    this->addChild(node);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = std::bind(&GameEndLayer::hide, this, std::placeholders::_1, std::placeholders::_2);
    auto dispatcher = this->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void
GameEndLayer::show(int64_t uid, int score)
{
    auto end = GameEndLayer::create();
    gGameMenu->addChild(end);
    
    User* user = base::Singleton<User>::get();
    
    Node* nd = nullptr;
    if(user->uid == uid)
    {
        nd = end->node->getChildByName("Node_Victory");
        auto glitter = nd->getChildByName("Sprite_Glitter");
        glitter->runAction(RepeatForever::create(RotateBy::create(10, 360)) );
    }else
    {
        nd = end->node->getChildByName("Node_Failure");
    }
    nd->setVisible(true);
    auto txScore = static_cast<ui::Text*>(end->node->getChildByName("Text_Score")) ;
    txScore->setString(StringUtils::format("%d", score));
    
    end->node->setPositionX(gCenter.x);
    end->node->runAction(EaseElasticOut::create(MoveTo::create(1, gCenter)) );
}

bool
GameEndLayer::hide(Touch* touch, Event* event)
{
    ActionQueue* queue = base::Singleton<ActionQueue>::get();
    queue->playNext();
    
    gMenuLayer->navLayer->setVisible(true);
    gMenuLayer->hallLayer->btnBegin->setEnabled(true);
    gGameScene->reset();
    gGameLayer->setVisible(false);
    this->removeFromParent();
    
    return true;
}
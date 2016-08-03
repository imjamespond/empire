//
//  LoadingLayer.cpp
//  empire
//
//  Created by metasoft on 16/5/6.
//
//

#include "LoadingLayer.hpp"
#include "GameLayer.h"
#include "GameSceneLayer.h"
#include "GameMenuLayer.hpp"
#include "MenuLayer.h"
#include "LoginLayer.h"
#include "NotificationLayer.h"
#include "../resource.h"
#include "../model/ActionSprite.h"
#include "../model/CardLayer.hpp"
#include "../codechiev/Network.h"
#include "../codechiev/BaseCmd.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/base/TimeUtil.h"
#include "../AudioManager.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace codechiev;

void
LoadingLayer::onOpen()
{
    LoginLayer::showLogin();

    //MessageBox("Congrats on completing the game!", "Victory");
    
    if(gLoadingLayer)
    {
        gLoadingLayer->removeFromParent();
        gLoadingLayer=nullptr;
    }
}

LoadingLayer *gLoadingLayer(nullptr);
LoadingLayer::LoadingLayer():loadingBar(nullptr),loadingNode(nullptr),percentage(0){}

int initData(7);
base::Time pretime(0);
bool
LoadingLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    loadingNode = CSLoader::createNode(kLoadingLayer);
    loadingNode->setPosition(gCenter);
    this->addChild(loadingNode);
    loadingBar = static_cast<ui::LoadingBar*>(loadingNode->getChildByName("LoadingBar"));
    percentBMF = static_cast<ui::TextBMFont*>(loadingNode->getChildByName("BMF_Percent"));
    
    this->scheduleUpdate();
    pretime = base::Time::now().getMillis();

    return true;
}

void
LoadingLayer::update(float dt)
{
    //base::Time now = base::Time::now();
    //log("now %lld",now.getMillis());
    //percentage+=(now-pretime)*.02;
    //pretime=now.getMillis();
    percentage+=dt*30;
    loadingBar->setPercent(percentage);
    percentBMF->setString(StringUtils::format("%d", static_cast<int>(percentage)));
    
    if(percentage>30&&initData&1)
    {
        initData^=1;
        onLoading();
    }
    else if(percentage>50&&initData&2)
    {
        initData^=2;
        
        if(!gGameLayer)
        {
            gGameLayer = GameLayer::create();
            gGameLayer->setPosition(gOrigin);
            gGameLayer->setVisible(false);
            gScene->addChild(gGameLayer,1);
            
            //======registEvent======
            codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.game.expired", std::bind( &GameMenuLayer::expiredEvent, gGameLayer->gameMenu, std::placeholders::_1));
            //======registEvent======
            base::Singleton<CmdCallback>::get()->registEvent("event.game.combat", std::bind( &GameSceneLayer::combatEvent, gGameLayer->gameScene, std::placeholders::_1));
        }
        if(!gMenuLayer)
        {
            gMenuLayer = MenuLayer::create();
            gMenuLayer->setPosition(gOrigin);
            gMenuLayer->setVisible(false);
            gScene->addChild(gMenuLayer,2);
            
            //======registEvent======
            codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.game", std::bind( &MenuLayer::gameEvent, gMenuLayer, std::placeholders::_1));
            //======registEvent======
            codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.game.end", std::bind( &MenuLayer::gameEndEvent, gMenuLayer, std::placeholders::_1));
            //======registEvent======
            codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.update", std::bind( &MenuLayer::updateEvent, gMenuLayer, std::placeholders::_1));
        }

    }
    else if(percentage>70&&initData&4)
    {
        initData^=4;
        
        //connect
        base::Singleton<Network>::get()->connect(std::bind(&LoadingLayer::onOpen, this));
    }
    else if(percentage>100)
    {
        this->unscheduleUpdate();
    }
    
}

void
LoadingLayer::onLoading()
{
    gNotificationLayer = NotificationLayer::create();
    gNotificationLayer->setPosition(gOrigin);
    gScene->addChild(gNotificationLayer, 9999);
    
    //init animation data
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kPlistEffect);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kPlistCard);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kPlistRole);
    
    base::Singleton<AnimationConfig>::get()->init();
    base::Singleton<CardConfig>::get()->init();

    //play audio
    if(base::Singleton<BasisUtil>::get()->getAppBool(AK_MUTE))
    {
        float vol = base::Singleton<BasisUtil>::get()->getAppFloat(AK_VOL);
        AudioManager::setVolumn(vol==0?1:vol);
        AudioManager::play(AD_BACKGROUND);
    }
}

#include "MenuLayer.h"
#include "GameLayer.h"
#include "GameSceneLayer.h"
#include "GameMenuLayer.hpp"
#include "LoadingLayer.hpp"
#include "NotificationLayer.h"
#include "../model/ConfirmLayer.hpp"
#include "../model/Configure.hpp"
#include "../model/NavigateLayer.hpp"
#include "../model/ActionSprite.h"
#include "../model/CardLayer.hpp"
#include "../model/HallLayer.hpp"
#include "../model/ActionQueue.hpp"
#include "../model/GameEndLayer.hpp"
#include "../model/RankLayer.hpp"
#include "../model/RedeemLayer.hpp"
#include "../model/PvpSearchLayer.hpp"
#include "../model/UpgradeOkLayer.hpp"
#include "../model/ExchangeLayer.hpp"
#include "../codechiev/Network.h"
#include "../codechiev/BasisUtil.h"

#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace codechiev;

MenuLayer::MenuLayer():navLayer(nullptr),pvpSearchLayer(nullptr){}
MenuLayer::~MenuLayer(){log("~MenuLayer");}

bool
MenuLayer::init()
{
    game_ = base::Singleton<Game>::get();
    
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
    
    //Main UI
    navLayer = NavigateLayer::create();
    navLayer->initNav(this);
    this->addChild(navLayer);
    
    //3 ui
    ui::Widget* widget2 = navLayer->pageView->getItem(1);
    hallLayer = HallLayer::create();
    hallLayer->setPosition(Vec2::ZERO);
    widget2->addChild(hallLayer);
    
    ui::Widget* widget1 = navLayer->pageView->getItem(0);
    cardLayer = CardLayer::create();
    cardLayer->setPosition(Vec2::ZERO);
    widget1->addChild(cardLayer);
    
    ui::Widget* widget3 = navLayer->pageView->getItem(2);
    msgLayer = MessageLayer::create();
    msgLayer->setPosition(Vec2::ZERO);
    widget3->addChild(msgLayer);
    
    return true;
}

void
MenuLayer::onEnter()
{
	Layer::onEnter();
	scheduleUpdate();
}

void
MenuLayer::update(float delta)
{
	Layer::update( delta);
}

void
MenuLayer::gameEvent(rapidjson::Document &doc)
{
    if (gGameLayer && !game_->isBegin())
    {
        hallLayer->stopCountDown();
        
        if(pvpSearchLayer)
        {
            pvpSearchLayer->undoModal();
        }
        
        navLayer->setVisible(false);
        game_->initGame(doc);
        
        gGameLayer->gameMenu->initGameMenu();//before scene because of removing previous oper btn
        gGameLayer->gameScene->initScene();
        
        gGameLayer->setVisible(true);
    }
}

void
MenuLayer::gameEndEvent(rapidjson::Document &doc)
{
    if(game_->state & Game::STATE_END)
    {
        return;
    }
    
    if(gGameLayer)
    {
        game_->state = Game::STATE_END;
        
        const rapidjson::Value& data = doc["data"];
        const rapidjson::Value& score = data["score"];
        const rapidjson::Value& uid = data["uid"];
        
        ActionQueue* queue = base::Singleton<ActionQueue>::get();
        if(queue->actionQueue.size())
        {
            queue->addQueue(boost::bind(&GameEndLayer::show, uid.GetInt64(), score.GetInt()));
        }else
        {
            gGameLayer->gameMenu->timerStop();
            GameEndLayer::show(uid.GetInt64(), score.GetInt());
        }
    }
}

void
MenuLayer::updateEvent(rapidjson::Document &doc)
{
    const rapidjson::Value& data = doc["data"];
    assert(!data["score"].IsNull());
    int score = data["score"].GetInt();
    
    gNotificationLayer->onError(StringUtils::format("update score: %d", score));
    
    hallLayer->updateScore(score);
}

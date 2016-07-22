//
//  HallLayer.cpp
//  empire
//
//  Created by metasoft on 16/5/11.
//
//

#include "HallLayer.hpp"
#include "MessageLayer.hpp"
#include "ActionQueue.hpp"
#include "RankLayer.hpp"
#include "Configure.hpp"
#include "CardLayer.hpp"
#include "CycleShowPageView.hpp"
#include "NavigateLayer.hpp"
#include "CountDown.hpp"
#include "ClipLayer.hpp"
#include "PvpSearchLayer.hpp"
#include "ExchangeLayer.hpp"
#include "RecieveOkLayer.hpp"
#include "User.h"
#include "../resource.h"
#include "../layer/GameLayer.h"
#include "../layer/MenuLayer.h"
#include "../layer/NotificationLayer.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/control/GameController.hpp"
#include "../codechiev/control/LoginController.h"

#include <cocostudio/CocoStudio.h>
#include <boost/bind.hpp>

USING_NS_CC;
using namespace codechiev;

HallLayer::HallLayer(){}

bool
HallLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    auto hallLayer = CSLoader::createNode(kHallLayer);
    hallLayer->setPosition(gCenter);
    this->addChild(hallLayer);
    
    txName = static_cast<ui::Text*>(hallLayer->getChildByName("TX_Name"));
    txLevel = static_cast<ui::Text*>(hallLayer->getChildByName("TX_Level"));
    txScore = static_cast<ui::Text*>(hallLayer->getChildByName("TX_Score"));
    
    btnBegin = static_cast<ui::Button*>(hallLayer->getChildByName("BTN_Begin"));
    btnBegin->addClickEventListener(CC_CALLBACK_1( HallLayer::gameAlloc, this));
    
    btnConfig = static_cast<ui::Button*>(hallLayer->getChildByName("BTN_Config"));
    btnConfig->addClickEventListener(std::bind([=](cocos2d::Ref*){
        //config panel
        auto configLayer = Configure::create();
        configLayer->setPosition(Vec2::ZERO);
        configLayer->setVisible(false);
        gMenuLayer->addChild(configLayer);
        configLayer->editPasswd->setText(base::Singleton<BasisUtil>::get()->getAppString(AK_PASSWD).c_str());
        configLayer->doModal();
    }, std::placeholders::_1));
    
    auto btnRecv = static_cast<ui::Button*>(hallLayer->getChildByName("BTN_Recv"));
    btnRecv->addClickEventListener(std::bind(&HallLayer::onRecieve, this, std::placeholders::_1));
    
    auto btnRank = static_cast<ui::Button*>(hallLayer->getChildByName("BTN_Rank"));
    btnRank->addClickEventListener(std::bind(&HallLayer::onRanklist, this, std::placeholders::_1));
    auto btnIntro = static_cast<ui::Button*>(hallLayer->getChildByName("BTN_Intro"));
    btnIntro->addClickEventListener(std::bind(&HallLayer::onIntro, this, std::placeholders::_1));
    auto btnExchange = static_cast<ui::Button*>(hallLayer->getChildByName("BTN_Exchange"));
    btnExchange->addClickEventListener(std::bind(&HallLayer::onExchange, this, std::placeholders::_1));
    
    auto mapFrame = static_cast<Node*>(hallLayer->getChildByName("MapFrame"));
    pageViewMap = CycleShowPageView::create();
    pageViewMap->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mapFrame->addChild(pageViewMap);
    
    for(int i=0; i<4; i++)
    {
        auto map = MapImgView::create();
        map->retain();//for page turnning
        map->init(StringUtils::format(kImgThumbMaps, i));
        map->mapid = i;
        pageViewMap->setContentSize(map->getContentSize());
        pageViewMap->addPage(map);
    }
    
    //==========update user information event================
    auto listener = EventListenerCustom::create(kUserEvent, [=](EventCustom* event){
        User *self = base::Singleton<User>::get();
        txName->setString(self->name);
        updateLevel(self->level);
        updateScore(self->score);
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    
    return true;
}

void
HallLayer::gameAlloc(Ref* ref)
{
    //ui::Button* btn = static_cast<ui::Button*>(ref);
    
    codechiev::control::GameController::roles_vec roles;
    CardUnit **cardsel = gMenuLayer->cardLayer->cardsel;
    roles.push_back(cardsel[0]->frame->id);
    roles.push_back(cardsel[1]->frame->id);
    roles.push_back(cardsel[2]->frame->id);
    roles.push_back(cardsel[3]->frame->id);
    int mapid(0);
    auto page = pageViewMap->getItem(pageViewMap->getCurrentPageIndex());
    if(page)
        mapid = static_cast<MapImgView*>(page)->mapid;
    codechiev::control::GameController::allocate(mapid, roles,
    std::bind([=](HallLayer* hall,rapidjson::Document& doc, bool ok){
        if(ok&&doc["data"].GetBool())
        {
            auto pvpSearchLayer = PvpSearchLayer::create();
            pvpSearchLayer->setVisible(false);
            gMenuLayer->addChild(pvpSearchLayer);
            gMenuLayer->pvpSearchLayer = pvpSearchLayer;
            pvpSearchLayer->doModal();
            //log("gameAlloc ok");
        }

    }, this, std::placeholders::_1, std::placeholders::_2));
}
void
HallLayer::gamePveAlloc()
{
    codechiev::control::GameController::roles_vec roles;
    CardUnit **cardsel = gMenuLayer->cardLayer->cardsel;
    roles.push_back(cardsel[0]->frame->id);
    roles.push_back(cardsel[1]->frame->id);
    roles.push_back(cardsel[2]->frame->id);
    roles.push_back(cardsel[3]->frame->id);
    
    int mapid(0);
    auto page = pageViewMap->getItem(pageViewMap->getCurrentPageIndex());
    if(page)
        mapid = static_cast<MapImgView*>(page)->mapid;
    codechiev::control::GameController::pveAlloc(mapid, roles, nullptr);
    
    stopCountDown();
}

void
HallLayer::onRanklist(Ref* ref)
{
    auto rankLayer = RankLayer::create();
    rankLayer->setVisible(false);
    gMenuLayer->addChild(rankLayer);
    rankLayer->doModal();
}

void
HallLayer::onRecieve(cocos2d::Ref *)
{
    control::LoginController::recieve(CmdCallbackBeginWithData
      if(ok)
      {
          gNotificationLayer->onError("领取成功");
          RecieveOkLayer::show(gMenuLayer, data);
      }
              CmdCallbackEnd);
}

void
HallLayer::onIntro(cocos2d::Ref *)
{
    auto introLayer = IntroLayer::create();
    introLayer->setVisible(false);
    gMenuLayer->addChild(introLayer);
    introLayer->doModal();
}

void
HallLayer::onExchange(cocos2d::Ref *)
{
    auto exchangeLayer = ExchangeLayer::create();
    exchangeLayer->setVisible(false);
    gMenuLayer->addChild(exchangeLayer);
    exchangeLayer->doModal();
}

void
HallLayer::stopCountDown()
{
    
}

void
HallLayer::updateLevel(int level)
{
    User *self = base::Singleton<User>::get();
    self->level = level;
    txLevel->setString(StringUtils::format("%d",level));
}
void
HallLayer::updateScore(int score)
{
    User *self = base::Singleton<User>::get();
    self->score = score;
    txScore->setString(StringUtils::format("%d",score));
}

bool
IntroLayer::init()
{
    if(!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kIntroLayer)));
    
    auto close=static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Close"));
    close->addClickEventListener(std::bind(&IntroLayer::onClose, this, std::placeholders::_1));
    
    return true;
}

//
//  GameMenuLayer.cpp
//  empire
//
//  Created by metasoft on 16/4/19.
//
//

#include "GameMenuLayer.hpp"
#include "GameSceneLayer.h"
#include "GameLayer.h"
#include "../codechiev/BasisUtil.h"
#include "../model/ScreenShake.hpp"
#include "../model/CountDown.hpp"
#include "../model/ClipLayer.hpp"
#include "../model/RoleLayer.hpp"
#include "../model/GameEndLayer.hpp"
#include "../codechiev/control/GameController.hpp"
#include "../resource.h"
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace codechiev;

bool
GameMenuLayer::init()
{
    game = base::Singleton<Game>::get();
    
    if (!Layer::init())
    {
        return false;
    }
    
    battleMenu = CSLoader::createNode(kBattleMenu);
    battleMenu->setPosition(Vec2::ZERO);
    this->addChild(battleMenu);
    
    Game::Player *player0 = &game->player0;
    Game::Player *player1 = &game->player1;
    
    //info pannel
    ready = battleMenu->getChildByName("Node_Ready");
    //info->setPosition(gCenter);
    player0->txPlayer = static_cast<ui::Text*>(ready->getChildByName("Node0")->getChildByName("TX_P0"));
    player1->txPlayer = static_cast<ui::Text*>(ready->getChildByName("Node1")->getChildByName("TX_P1"));
    
    Node* infoOfScene = gGameScene->battleLayer->getChildByName("Node_Info");
    
    player0->healthBar = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBar_0"));
    player1->healthBar = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBar_1"));
    
    player0->healthBarBg = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBarBg_0"));
    player1->healthBarBg = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBarBg_1"));
    
    player0->txHealth = static_cast<ui::Text*>(infoOfScene->getChildByName("Tx_P0_Health"));
    player1->txHealth = static_cast<ui::Text*>(infoOfScene->getChildByName("Tx_P1_Health"));
    
    blue = static_cast<Sprite*>(battleMenu->getChildByName("Blue"));
    red = static_cast<Sprite*>(battleMenu->getChildByName("Red"));
    player0->txTimer = static_cast<ui::Text*>(red->getChildByName("Text_Timer"));
    player1->txTimer = static_cast<ui::Text*>(blue->getChildByName("Text_Timer"));
    
    for(int i=0; i<(sizeof player0->opers)/sizeof(oper_struct); i++)
    {
        player0->opers[i].node = red->getChildByName(StringUtils::format("Node_1_%d", i));
        player1->opers[i].node = blue->getChildByName(StringUtils::format("Node_1_%d", i));
    }

    return true;
}

void
GameMenuLayer::initGameMenu()
{
    //banner
    auto redbanner = static_cast<Sprite*>(red->getChildByName("Sprite_Banner"));
    auto bluebanner = static_cast<Sprite*>(blue->getChildByName("Sprite_Banner"));
    redbanner->initWithFile(StringUtils::format("img/oper_%d.png", game->map) );
    bluebanner->initWithFile(StringUtils::format("img/oper_%d.png", game->map) );
    redbanner->setFlippedY(true);
    
    Game::Player *player0 = &game->player0;
    Game::Player *player1 = &game->player1;
    player0->txPlayer->setString(game->player0.name) ;
    player1->txPlayer->setString(game->player1.name) ;
    ready->setVisible(true);
    ready->runAction(Sequence::create(DelayTime::create(1),
                                      CallFunc::create(std::bind([=](Node* node){
        node->setVisible(false);
    }, ready)), nullptr));
    
    if(player0->uid<0)
        player0->txTimer->setVisible(false);
    if(player1->uid<0)
        player1->txTimer->setVisible(false);
    
    updateHealth(game->player0, game->player0.hp, game->player0.maxHp);
    updateHealth(game->player1, game->player1.hp, game->player1.maxHp);

    for(int i=0; i<(sizeof player0->opers)/sizeof(oper_struct); i++)
    {
        if(player0->opers[i].btn)
        {
            player0->opers[i].btn->removeFromParent();
            player0->opers[i].btn=nullptr;
        }
        if(player1->opers[i].btn)
        {
            player1->opers[i].btn->removeFromParent();
            player1->opers[i].btn=nullptr;
        }
    }
}

void
GameMenuLayer::updateHealth(const Game::Player& player, int hp, int maxhp)
{
    float proportion = static_cast<float>(hp)/static_cast<float>(maxhp);
    float red = 255*(3-proportion*4);
    float green = 255*(proportion*4-1);
    player.healthBar->setPercent(proportion*100);
    player.healthBar->setColor( Color3B(MIN(255,MAX(0,red)), MIN(255,MAX(0,green)), 0));
    player.healthBarBg->setOpacity(255);
    player.healthBarBg->runAction( Sequence::create(FadeOut::create(1.0f),
                                    CallFunc::create(std::bind([](const Game::Player* player){
        player->healthBarBg->setPercent(player->healthBar->getPercent());
    }, &player )), nullptr) );
    player.txHealth->setString(StringUtils::format("%d", hp) );
}

void
GameMenuLayer::showSwapMenu(bool show)
{
    red->setPositionY(gVisibleSize.y+red->getContentSize().height);
    blue->setPositionY(-blue->getContentSize().height);
    
    if(show)
    {
        int turn = game->turn;
        if(turn==0)
        {
            addSwapMenu(0);
            addSwapMenu(1);
        }else if(turn == 1)
        {
            addSwapMenu(2);
            addSwapMenu(3);
        }else if(turn == 2)
        {
            addSwapMenu(4);
            addSwapMenu(5);
        }else if(turn == 3)
        {
            addSwapMenu(6);
            addSwapMenu(7);
        }
        
        red->runAction(MoveTo::create(1.0f,Vec2(0,gVisibleSize.y)));
        blue->runAction(MoveTo::create(1.0f,Vec2::ZERO));
    }
}

void
GameMenuLayer::addSwapMenu(int pos)
{
    //Game::Player* self = game->getSelf();
    Game::Player* enemy = game->getEnemy();

    for(int i=0; i<(sizeof game->player0.opers)/sizeof(oper_struct); i++)
    {
        if(!game->player0.opers[i].btn)
        {
            auto sprite = SwapBtn::create();
            if(enemy&&enemy==&game->player0)
                sprite->getListener()->setEnabled(false);
            sprite->type = game->player0.swap[pos];
            sprite->setTexture(StringUtils::format(kImgRedOper, game->player0.swap[pos]));
            game->player0.opers[i].btn = sprite;
            game->player0.opers[i].node->addChild(sprite) ;
            break;
        }
    }
    for(int i=0; i<(sizeof game->player1.opers)/sizeof(oper_struct); i++)
    {
        if(!game->player1.opers[i].btn )
        {
            auto sprite = SwapBtn::create();
            if(enemy&&enemy==&game->player1)
                sprite->getListener()->setEnabled(false);
            sprite->type = game->player1.swap[pos];
            sprite->setTexture(StringUtils::format(kImgBlueOper, game->player1.swap[pos]));
            game->player1.opers[i].btn = sprite;
            game->player1.opers[i].node->addChild(sprite) ;
            break;
        }
    }
}

void
GameMenuLayer::expiredEvent(rapidjson::Document &doc)
{
    const rapidjson::Value& data = doc["data"];
    assert(!data["hp"].IsNull() && !data["uid"].IsNull());
    int hp = data["hp"].GetInt();
    int64_t uid = data["uid"].GetInt64();
    Game::Player* player = game->getPlayer(uid);
    if(player)
    {
        player->hp = hp;
        updateHealth(*player, hp, player->maxHp);
    }
}
void
GameMenuLayer::removeOperBtn(Game::Player& player, int type)
{
    for(int i=0; i<(sizeof player.opers)/sizeof(oper_struct) ; i++)
    {
        if(player.opers[i].btn && player.opers[i].btn->type == type)
        {
            player.opers[i].btn->removeFromParent();
            player.opers[i].btn = nullptr;
        }
    }
}


bool
SwapBtn::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    
    auto ls = EventListenerTouchOneByOne::create();
    ls->onTouchBegan = [=](Touch* touch, Event* event){
        auto target = static_cast<SwapBtn*>(event->getCurrentTarget());
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode))
        {
            gGameScene->sendSwap(target->type);
            gGameScene->stopTimer();
            gGameMenu->showSwapMenu(false);
            target->getListener()->setEnabled(false);
            return true;
        }
        return false;
    };
    listener=ls;
    
    auto disp = this->getEventDispatcher();
    disp->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void
GameMenuLayer::timerBegin(float repeat)
{
    Game::Player* self = game->getSelf();
    assert(self);
    self->opertime = repeat;
    
    updateTimerText();
    this->schedule(SEL_SCHEDULE(&GameMenuLayer::updateTimer), 1);
}
void
GameMenuLayer::timerStop()
{
    this->unschedule(SEL_SCHEDULE(&GameMenuLayer::updateTimer));
}
void
GameMenuLayer::updateTimerText()
{
    Game::Player* self = game->getSelf();
    assert(self);
    self->txTimer->setString(StringUtils::format("%g",self->opertime));
    if(self->opertime<=5)
    {
        self->txTimer->runAction(Sequence::createWithTwoActions(ScaleBy::create(0.05f, 1-.3f),ScaleTo::create(0.125f, 1)));
    }
}
void
GameMenuLayer::updateTimer(float dt)
{
    Game::Player* self = game->getSelf();
    assert(self);
    --self->opertime;
    //Animation
    if(self->opertime)
    {
        updateTimerText();
    }else if(self->opertime==0)
    {
        gGameScene->onTimeup();
        self->opertime=2;
        updateTimerText();
    }else if(self->opertime<0)
    {
        timerStop();
    }
}


bool
GameMenuLayer::checkguidepassed()
{
    if(base::Singleton<BasisUtil>::get()->getAppBool(AK_GUIDE_PASSED) )
        return true;
    //else
    base::Singleton<BasisUtil>::get()->setAppBool(AK_GUIDE_PASSED, true);
    red->setPosition(Vec2(0,gVisibleSize.y));
    blue->setPosition(Vec2::ZERO);
    
    auto btnGuide1 = static_cast<ui::Button*>(battleMenu->getChildByName("Button_Guide1"));
    btnGuide1->setVisible(true);
    auto btnGuide2 = static_cast<ui::Button*>(battleMenu->getChildByName("Button_Guide2"));
    auto btnGuide3 = static_cast<ui::Button*>(battleMenu->getChildByName("Button_Guide3"));
    btnGuide1->addClickEventListener(std::bind([=](Node* btn, Node* next){
        btn->setVisible(false);
        next->setVisible(true);}, btnGuide1, btnGuide2));
    btnGuide2->addClickEventListener(std::bind([=](Node* btn, Node* next){
        btn->setVisible(false);
        next->setVisible(true);}, btnGuide2, btnGuide3));
    btnGuide3->addClickEventListener(std::bind([=](Node* btn){
        btn->setVisible(false);
        gGameScene->startTimer();
    }, btnGuide3));
    return false;
}

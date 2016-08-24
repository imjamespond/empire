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
    
    auto battleMenu = CSLoader::createNode(kBattleMenu);
    battleMenu->setPosition(Vec2::ZERO);
    this->addChild(battleMenu);
    
    //info pannel
    Node* info = battleMenu->getChildByName("Node_Info");
    //info->setPosition(gCenter);
    
    Node* infoOfScene = gGameScene->battleLayer->getChildByName("Node_Info");;
    
    Game::Player *player0 = &game->player0;
    Game::Player *player1 = &game->player1;
    player0->healthBar = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBar_0"));
    player1->healthBar = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBar_1"));
    
    player0->healthBarBg = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBarBg_0"));
    player1->healthBarBg = static_cast<ui::LoadingBar*>(infoOfScene->getChildByName("LoadingBarBg_1"));
    
    player0->txPlayer = static_cast<ui::Text*>(info->getChildByName("TX_P0"));
    player1->txPlayer = static_cast<ui::Text*>(info->getChildByName("TX_P1"));
    
    player0->txHealth = static_cast<ui::TextBMFont*>(infoOfScene->getChildByName("TBMF_P0_Health"));
    player1->txHealth = static_cast<ui::TextBMFont*>(infoOfScene->getChildByName("TBMF_P1_Health"));
    
    blue = static_cast<Sprite*>(battleMenu->getChildByName("Blue"));
    red = static_cast<Sprite*>(battleMenu->getChildByName("Red"));
    player0->txTimer = static_cast<ui::Text*>(red->getChildByName("Text_Timer"));
    player1->txTimer = static_cast<ui::Text*>(blue->getChildByName("Text_Timer"));
    
    for(int i=0; i<(sizeof redopers)/sizeof(oper_struct); i++)
    {
        redopers[i].node = red->getChildByName(StringUtils::format("Node_1_%d", i));
    }
    for(int i=0; i<(sizeof blueopers)/sizeof(oper_struct); i++)
    {
        blueopers[i].node = blue->getChildByName(StringUtils::format("Node_1_%d", i));
    }
    
    return true;
}

void
GameMenuLayer::initGameMenu()
{
    Game::Player *player0 = &game->player0;
    Game::Player *player1 = &game->player1;
    player0->txPlayer->setString(game->player0.name) ;
    player1->txPlayer->setString(game->player1.name) ;
    
    if(player0->uid<0)
        player0->txTimer->setVisible(false);
    if(player1->uid<0)
        player1->txTimer->setVisible(false);
    
    updateHealth(game->player0, game->player0.hp, game->player0.maxHp);
    updateHealth(game->player1, game->player1.hp, game->player1.maxHp);

    for(int i=0; i<(sizeof redopers)/sizeof(oper_struct); i++)
    {
        if(redopers[i].btn)
        {
            redopers[i].btn->removeFromParent();
            redopers[i].btn=nullptr;
        }
    }
    for(int i=0; i<(sizeof blueopers)/sizeof(oper_struct); i++)
    {
        if(blueopers[i].btn)
        {
            blueopers[i].btn->removeFromParent();
            blueopers[i].btn=nullptr;
        }
    }
}

void
GameMenuLayer::updateHealth(const codechiev::Game::Player& player, int hp, int maxhp)
{
    float proportion = static_cast<float>(hp)/static_cast<float>(maxhp);
    float red = 255*(3-proportion*4);
    float green = 255*(proportion*4-1);
    player.healthBar->setPercent(proportion*100);
    player.healthBar->setColor( Color3B(MIN(255,MAX(0,red)), MIN(255,MAX(0,green)), 0));
    player.healthBarBg->setOpacity(255);
    player.healthBarBg->runAction( Sequence::create(FadeOut::create(1.0f),
                                            CallFunc::create(std::bind([](const codechiev::Game::Player* player){
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
    Game::Player* self = game->getSelf();
    Game::Player* enemy = game->getEnemy();

    for(int i=0; i<(sizeof redopers)/sizeof(oper_struct); i++)
    {
        if(!redopers[i].btn &&self)
        {
            auto sprite = SwapBtn::create();
            sprite->type = self->swap[pos];
            sprite->setTexture(StringUtils::format(kImgRedOper, self->swap[pos]));
            redopers[i].btn = sprite;
            redopers[i].node->addChild(sprite) ;
            break;
        }
    }
    for(int i=0; i<(sizeof blueopers)/sizeof(oper_struct); i++)
    {
        if(!blueopers[i].btn &&enemy)
        {
            auto sprite = SwapBtn::create();
            sprite->getListener()->setEnabled(false);
            sprite->type = enemy->swap[pos];
            sprite->setTexture(StringUtils::format(kImgBlueOper, enemy->swap[pos]));
            blueopers[i].btn = sprite;
            blueopers[i].node->addChild(sprite) ;
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
GameMenuLayer::removeBlueBtn(int type)
{
    for(int i=0; i<(sizeof blueopers)/sizeof(oper_struct) ; i++)
    {
        if(blueopers[i].btn && blueopers[i].btn->type == type)
        {
            blueopers[i].btn->removeFromParent();
            blueopers[i].btn = nullptr;
        }
    }
}
void
GameMenuLayer::removeRedBtn(int type)
{
    for(int i=0; i<(sizeof redopers)/sizeof(oper_struct) ; i++)
    {
        if(redopers[i].btn && redopers[i].btn->type == type)
        {
            redopers[i].btn->removeFromParent();
            redopers[i].btn = nullptr;
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
            gGameLayer->gameMenu->showSwapMenu(false);
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


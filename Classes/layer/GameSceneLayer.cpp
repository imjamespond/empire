#include "GameSceneLayer.h"
#include "GameLayer.h"
#include "GameMenuLayer.hpp"
#include "NotificationLayer.h"
#include "../resource.h"
#include "../model/Particle.h"
#include "../model/ClipLayer.hpp"
#include "../model/CountDown.hpp"
#include "../model/ActionSprite.h"
#include "../model/ActionQueue.hpp"
#include "../model/Effect.hpp"
#include "../model/RoleSprite.hpp"

#include "../codechiev/BasisUtil.h"
#include "../codechiev/BaseCmd.h"
#include "../codechiev/control/GameController.hpp"
#include <cocostudio/CocoStudio.h>
#include <boost/bind.hpp>

USING_NS_CC;
using namespace codechiev;
using namespace rapidjson;

#define ROUND_COUNT 10

GameSceneLayer::GameSceneLayer() :
battleLayer(nullptr),
sendingSwap_(false),
map(nullptr),
sceneSize(0, 0)
{}

GameSceneLayer::~GameSceneLayer()
{}

void 
GameSceneLayer::onExit()
{
	Layer::onExit();
}


bool
GameSceneLayer::init()
{
    game = base::Singleton<Game>::get();
    
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
    
    battleLayer = CSLoader::createNode(kBattleLayer);
    this->addChild(battleLayer,99);
    
	//sonic = Sonic::create();
    //sonic->getEmitter()->stopSystem();
	//this->addChild(sonic);
	return true;
}

void
GameSceneLayer::initScene()
{
    initTiledMap();
    
    initPlayer(&game->player0,0);
    initPlayer(&game->player1,1);
    
    //timer
    startTimer();
    
    gGameLayer->cameraBoundUpdate(sceneSize, 1.0f);
    gGameLayer->cameraReset();
}
void
GameSceneLayer::initPlayer(Game::Player *player, int pos)
{
    for(int i=0; i<4; i++)
    {
        Role** roles = player->roles;
        assert(roles[i]->id);
        std::string name;
        if(pos)
        {
            name = StringUtils::format("Node_%d",i+1);
            roles[i]->role->setFlippedX(true);
        }
        else
        {
            name = StringUtils::format("Node_%d",i+5);
        }
        roles[i]->setPosition( battleLayer->getChildByName(name)->getPosition());
        this->addChild(roles[i]);
    }
}
void
GameSceneLayer::initTiledMap()
{
    //tileMap_ = TMXTiledMap::create("map.tmx");
    //this->addChild(tileMap_, 0, kTagTileMap);
    //this->boundUpdate(Vec2(tileMap_->getContentSize().width, tileMap_->getContentSize().height));
    
    std::string name = StringUtils::format(kImgMaps, game->map);
    if(!FileUtils::getInstance()->isFileExist(name))
        name = StringUtils::format(kImgMaps, 0);
    map = Sprite::create(name);
    this->addChild(map);
    sceneSize = map->getContentSize();
    map->setPosition(gCenter);
}

void
GameSceneLayer::reset()
{
    if(game)
        game->resetScene();
    if(map)
        map->removeFromParent();
    map=nullptr;
}

void
GameSceneLayer::startTimer()
{
    sendingSwap_ = false;
    
    Game::Player* self = game->getSelf();
    if(self&&self->petrify==0)
    {
        gGameLayer->gameMenu->showSwapMenu();
    }else
    {
        sendSwap(0);
    }
    
    //check buff
    game->player0.checkPetrify();
    game->player1.checkPetrify();
    
    game->player0.checkFlipX();
    game->player1.checkFlipX();
    
    if(Game::STATE_BEGIN & game->state)
    {
        gGameLayer->gameMenu->timerBegin(ROUND_COUNT+self->prolong-self->shorten);
        self->prolong=0;
        self->shorten=0;
    }
}

void
GameSceneLayer::stopTimer()
{
    gGameLayer->gameMenu->timerStop();
}

void
GameSceneLayer::onTimeup()
{
    if(game->isBegin())
    {
        sendExpired();
    }
}

void
GameSceneLayer::sendSwap(int type)
{
    if(!sendingSwap_)
    {
        sendingSwap_ = true;
        control::GameController::swap(type, CmdCallbackBegin
        CmdCallbackEnd);
    }
}
void
GameSceneLayer::sendExpired()
{
    control::GameController::swap(-1, std::bind([=](rapidjson::Document& doc, bool ok){
    },std::placeholders::_1, std::placeholders::_2));
}

void
GameSceneLayer::swapAnim(std::vector<int> ids, Game::Player& player)
{
    if(ids.size() == 8)
    {
        for(int i(0); i<4; i++)
        {
            Role* role = player.getRole(ids[i*2]);
            Role* roleSwap = player.getRole(ids[i*2+1]);
            role->walkTo( roleSwap->getPosition(), 300);
            assert(role&&roleSwap);
        }
        
        this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(std::bind([=](){
            base::Singleton<ActionQueue>::get()->playNext();
        }) ), nullptr));
    }else if(ids.size() == 2)
    {
        Role* role0 = player.getRole(ids.at(0));
        Role* role1 = player.getRole(ids.at(1));
        assert(role0&&role1);
        float dur1 = role1->walkTo( role0->getPosition(), 300);
        float dur0 = role0->walkTo( role1->getPosition(), 300);
        //std::swap(roles[swap0].role, roles[swap1].role);
        
        auto cb = CallFunc::create(std::bind([=](){
            base::Singleton<ActionQueue>::get()->playNext();
        }) );
        this->runAction(Sequence::create(DelayTime::create(std::max(dur0, dur1)), cb, nullptr));
    }
}
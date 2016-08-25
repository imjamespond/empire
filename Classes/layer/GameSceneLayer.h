#pragma once

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "../model/RoleLayer.hpp"
#include "../model/Game.hpp"
#include "../model/CountDown.hpp"
#include "../model/Particle.h"

class ParticleSprite;
class GameSceneLayer :
	public cocos2d::Layer
{
public:
	GameSceneLayer();
	~GameSceneLayer();

	CREATE_FUNC(GameSceneLayer);

	virtual bool init();
	virtual void onExit();

	void initScene();
    void initPlayer(codechiev::Game::Player*, int);
	void initTiledMap();
    void reset();
	bool checkPosition(const cocos2d::Vec2&);
	cocos2d::Vec2 tileCoordForPosition(const cocos2d::Vec2&);
	//void repositionSprite(float dt);
    void combatEvent(const rapidjson::Document &);
    void combatAnimQueue();
    void combatAnimAdd(codechiev::Game::Player *, codechiev::Game::Player *,const rapidjson::Value&);
    void combatAnimEnd();
    void swapAnim(std::vector<int> , codechiev::Game::Player& );
    //void end();
    void onTimeup();
    void startTimer();
    void stopTimer();
    void sendSwap(int);
    void sendExpired();

	//variable
    //Sonic *sonic;
	//RoleLayer *man;
    //codechiev::CountDown *cd;
    codechiev::Game *game;
    
	//cocos2d::TMXTiledMap *tileMap;
    cocos2d::Sprite *map;
	cocos2d::Size sceneSize;
	//cocos2d::Vec2 sceneCenter;
    cocos2d::Node* battleLayer;
private:
    bool sendingSwap_;
};


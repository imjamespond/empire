#pragma once

#include "cocos2d.h"
#include "../codechiev/transform.h"

//class GameSceneLayer;
class GameLayer;
class GameSceneLayer;
class GameMenuLayer;

extern GameLayer *gGameLayer;
extern GameSceneLayer *gGameScene;
extern GameMenuLayer *gGameMenu;
extern cocos2d::Scene *gScene;
extern cocos2d::Scene *createGameScene();

class GameLayer :
	public cocos2d::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void onTouchesBegan(const std::vector< cocos2d::Touch * > &, cocos2d::Event *);
	virtual void onTouchesMoved(const std::vector< cocos2d::Touch * > &, cocos2d::Event *);
	virtual void onTouchesEnded(const std::vector< cocos2d::Touch * > &, cocos2d::Event *);
	virtual void onTouchesCancelled(const std::vector< cocos2d::Touch * > &, cocos2d::Event *);
	virtual void onEnter();
	virtual void update(float );
	virtual bool init();
	CREATE_FUNC(GameLayer);

	void cameraBoundUpdate(cocos2d::Size&, float);
	void cameraReset();
    void reset();

	//GameSceneLayer *gameScene;
    //GameMenuLayer *gameMenu;
    codechiev::Camera camera;
private:
	void touchScale(cocos2d::Touch*, cocos2d::Touch*);
    void sceneScale(const cocos2d::Vec2&, const cocos2d::Vec2&, const cocos2d::Vec2&, const cocos2d::Vec2&);
	void cameraFollow();
	void cameraMoveTo(const cocos2d::Vec2&);
	
	struct
	{
		cocos2d::Vec2 min;
		cocos2d::Vec2 max;
	} camBound_;

	bool cameraMoving_;
	bool cameraFollowing_;
};

static const char kIntroLayer[] = "ui/IntroLayer.csb";
static const char kRankNode[] = "ui/RankNode.csb";
static const char kRankLayer[] = "ui/RankLayer.csb";
static const char kRankPosLayer[] = "ui/RankPosLayer.csb";
static const char kBattleMenu[] = "ui/BattleMenu.csb";
static const char kBattleLayer[] = "ui/BattleLayer.csb";
static const char kLoadingLayer[] = "ui/LoadingLayer.csb";

static const char kMenuLayer[] = "ui/MenuLayer.csb";
static const char kConfigLayer[] = "ui/ConfigLayer.csb";
static const char kHallLayer[] = "ui/HallLayer.csb";
static const char kCardLayer[] = "ui/CardLayer.csb";
static const char kCardPosLayer[] = "ui/CardPosLayer.csb";
static const char kCardUnitLayer[] = "ui/CardUnitLayer.csb";
static const char kMsgLayer[] = "ui/MsgLayer.csb";
static const char kNavMenuLayer[] = "ui/NavigationMenuLayer.csb";

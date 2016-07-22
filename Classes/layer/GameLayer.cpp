#include "GameLayer.h"
#include "GameSceneLayer.h"
#include "GameMenuLayer.hpp"
#include "MenuLayer.h"
#include "LoadingLayer.hpp"
#include "NotificationLayer.h"

#include "../codechiev/BaseCmd.h"
#include "../codechiev/BasisUtil.h"
#include "../model/CardLayer.hpp"
#include "../AudioManager.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace codechiev;

extern Size designResolutionSize;
Scene *gScene(nullptr);
GameLayer *gGameLayer(nullptr);
MenuLayer *gMenuLayer(nullptr);
NotificationLayer *gNotificationLayer(nullptr);

Scene* createGameScene()
{
	base::Singleton<BasisUtil>::get()->appData();

	// 'scene' is an autorelease object
	gScene = Scene::create();
	gVisibleSize = Director::getInstance()->getVisibleSize();//return designResolutionSize

	gOrigin = Director::getInstance()->getVisibleOrigin();
	gRightBottom = Vec2(gVisibleSize.x, 0);
	gCenter = gVisibleSize * .5f;
	gLeftBottom = Vec2(0,0);
	gLeftTop = Vec2(0, gVisibleSize.y);
    
    if(!gLoadingLayer)
    {
        gLoadingLayer = LoadingLayer::create();
        gLoadingLayer->setPosition(gOrigin);
        gScene->addChild(gLoadingLayer, 9998);
    }
    
	// return the scene
	return gScene;
}

GameLayer::GameLayer(): camera(), gameScene(nullptr),
cameraMoving_(false), cameraFollowing_(false)
{
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::onTouchesCancelled, this);

	auto dispatcher = this->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameLayer::~GameLayer()
{
	log("~GameLayer");
}

bool
GameLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->scheduleUpdate();
	
	//gameScene
	gameScene = GameSceneLayer::create();
    gameScene->setAnchorPoint(Vec2::ZERO);
    gameScene->setPosition(Vec2::ZERO);//set orgin
    this->addChild(gameScene);
    
    //gameMenu
    gameMenu = GameMenuLayer::create();
    gameMenu->setAnchorPoint(Vec2::ZERO);
    gameMenu->setPosition(Vec2::ZERO);
    this->addChild(gameMenu);

	return true;
}

void
GameLayer::update(float delta)
{
    //cameraFollow();
    if (camera.getCameraDirty())
    {
        Vec2 pos(gameScene->getPosition());
        camera.transform(pos);
        gameScene->setPosition(pos);
        camera.resetCameraDirty();
    }
    //Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live".
    Layer::update(delta);
}

void
 GameLayer::onEnter()
 {
     Layer::onEnter();
 }
/*End Virtual Function Definetion*/

void
GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
	if (touches.size() == 1)
	{
        //convert touch location to node space location
        //Vec2 to = gameScene->man->convertToNodeSpace(touches[0]->getLocation());
        //gameScene->man->role->walkTo(to);
        //gameScene->sonic->setPosition(to);
	}
}

void
GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
	if (touches.size()==1)
	{
		Touch* touch = touches[0];
		Vec2 diff = touch->getDelta();
		Vec2 currentPos = camera.getPos() - diff;
		cameraMoveTo(currentPos);

		cameraMoving_ = true;
		cameraFollowing_ = false;
	}

	if (touches.size()==2)
	{
		touchScale(touches[0], touches[1]);
	}
}

void
GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
	cameraMoving_ = false;
}

void
GameLayer::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event)
{
}

void 
GameLayer::cameraBoundUpdate(Size &scenesize, float zoom)
{
	//when map set to center of scene initially
	camBound_.min.x = (gVisibleSize.x - scenesize.width)*.5f*zoom;
	camBound_.min.y = (gVisibleSize.y - scenesize.height)*.5f*zoom;
    camBound_.min.x = camBound_.min.x>0?0:camBound_.min.x;
    camBound_.min.y = camBound_.min.y>0?0:camBound_.min.y;
    
    float sceneWidth = scenesize.width<gVisibleSize.x?gVisibleSize.x:scenesize.width;
    float sceneHeight = scenesize.height<gVisibleSize.y?gVisibleSize.y:scenesize.height;
    camBound_.max.x = camBound_.min.x + sceneWidth*zoom - gVisibleSize.x;
    camBound_.max.y = camBound_.min.y + sceneHeight*zoom - gVisibleSize.y;
    camBound_.max.x = camBound_.max.x<0?0:camBound_.max.x;
    camBound_.max.y = camBound_.max.y<0?0:camBound_.max.y;
}

void 
GameLayer::touchScale(cocos2d::Touch* t1, cocos2d::Touch* t2)
{
    Vec2 preLoc1 = t1->getPreviousLocation();
    Vec2 preLoc2 = t2->getPreviousLocation();
    
    Vec2 loc1 = t1->getLocation();
    Vec2 loc2 = t2->getLocation();
    
    this->sceneScale(preLoc1, preLoc2, loc1, loc2);
}
void
GameLayer::sceneScale(const cocos2d::Vec2& preLoc1, const cocos2d::Vec2& preLoc2, const cocos2d::Vec2& loc1, const cocos2d::Vec2& loc2)
{

	float preLength = (preLoc1 - preLoc2).length();
	float length = (loc1 - loc2).length();
	float zoom = gameScene->getScale() + (length - preLength) * 0.005f;

	if (zoom > 5.0f) {
		zoom = 5.0f;
	}else if (zoom < 1.0f) {
		zoom = 1.0f;
	}
    
	cameraBoundUpdate(gameScene->sceneSize ,zoom);
	Vec2 from = zoom/ gameScene->getScale() * (camera.getPos() + gCenter)-gCenter;
	camera.setPos(from);
    gameScene->setScale(zoom);
	cameraReset();
}

void
GameLayer::cameraMoveTo(const Vec2& moveTo)
{
    if (moveTo.x < camBound_.min.x )
    {camera.setPosX(camBound_.min.x);}
    else if( moveTo.x > camBound_.max.x)
    {camera.setPosX(camBound_.max.x);}
	else
    {camera.setPosX(moveTo.x);}
    
    if (moveTo.y < camBound_.min.y )
    {camera.setPosY(camBound_.min.y);}
    else if( moveTo.y > camBound_.max.y)
    {camera.setPosY(camBound_.max.y);}
	else
	{camera.setPosY(moveTo.y);}
}

void
GameLayer::cameraReset()
{
	if (camera.getPos().x < camBound_.min.x)
	{
		camera.setPosX(camBound_.min.x);
	}
	else if (camera.getPos().x > camBound_.max.x)
	{
		camera.setPosX(camBound_.max.x);
	}
	/**/
	if (camera.getPos().y < camBound_.min.y)
	{
		camera.setPosY(camBound_.min.y);
	}
	else if (camera.getPos().y > camBound_.max.y)
	{
		camera.setPosY(camBound_.max.y);
	}
}
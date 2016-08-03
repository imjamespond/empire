#include "NotificationLayer.h"
#include "GameLayer.h"
#include "MenuLayer.h"
#include "AudioManager.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/Network.h"
#include "../codechiev/control/LoginController.h"
#include "../resource.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace codechiev;

NotificationLayer::NotificationLayer() :loadAction(nullptr),
debug(nullptr){}

NotificationLayer::~NotificationLayer()
{
    if(loadAction)
        loadAction->release();
	log("~NotificationLayer");
}
bool
NotificationLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    darkLayer = LayerColor::create(Color4B(0, 0, 0, 200));
    darkLayer->setVisible(false);
    this->addChild(darkLayer);
    
    loadSprite = Sprite::create(kImgLoading);
    loadSprite->setPosition(gCenter);
    loadSprite->setScale(2, 2);
    loadSprite->setVisible(false);
    loadAction = RepeatForever::create( RotateBy::create(2.0f, 360) );
    loadAction->retain();
    this->addChild(loadSprite);
    
	debug = Label::createWithSystemFont("", "arial", 20);
    debug->setPosition(gCenter);
    debug->setDimensions(gVisibleSize.x, gVisibleSize.y);
    debug->setAlignment(TextHAlignment::LEFT);
    //label_->enableGlow(Color4B::RED);
    //label_->enableOutline(Color4B::GREEN, 2);
    //label_->enableShadow();
    //add the label as a child to this layer
	this->addChild(debug);

	this->scheduleUpdate();//activate update
	return true;
}

float pingInterval(0.0f);
float pingExpired(false);
void
NotificationLayer::update(float delta)
{
	Layer::update(delta);

    /*BaseDebug* d = base::Singleton<BaseDebug>::get();
	if (d->dirty)
	{
		debug->setString(d->toString());
	}*/
    
    pingInterval+=delta;
    if(pingExpired&&pingInterval>15)
    {
        base::Singleton<BaseDebug>::get()->add("ping expired");
        pingInterval=0;
        base::Singleton<Network>::get()->close();
    }
    else if(pingInterval>60)
    {
        pingInterval=0;
        if(base::Singleton<Network>::get()->isConnected())
        {
            pingExpired=true;
            control::LoginController::ping(CmdCallbackBegin
                                           if(ok)
                                           {
                                               base::Singleton<BaseDebug>::get()->add("ping ok");
                                               pingExpired=false;
                                           }
                                           CmdCallbackEnd);
        }

    }
}

Label *reconnectLayer(nullptr);
float reconnectCountdown(10);
void
NotificationLayer::autoReconnect()
{
    pingInterval=0;
    pingExpired=false;
    
    this->schedule(SEL_SCHEDULE(&NotificationLayer::autoReconnectLayer), 1);
    //TODO reconnect layer
    reconnectLayer = Label::createWithSystemFont("即将自动连接伺服器", "arial", 24);
    reconnectLayer->setPosition(gCenter);
    this->addChild(reconnectLayer);
}

void
NotificationLayer::autoReconnectLayer(float dt)
{
    if(reconnectLayer)
    {
        reconnectLayer->setString(StringUtils::format("%g 秒后自动连接伺服器", reconnectCountdown));
        
        reconnectCountdown-=dt;
        if(reconnectCountdown<0)
        {
            this->unschedule(CC_SCHEDULE_SELECTOR(NotificationLayer::autoReconnectLayer));
            reconnectLayer->removeFromParent();
            reconnectLayer = nullptr;
            reconnectCountdown = 10;
            
            base::Singleton<Network>::get()->connect();
        }
    }
}

void
NotificationLayer::pauseEventListener()
{
    _eventDispatcher->pauseEventListenersForTarget(gGameLayer, true);
    _eventDispatcher->pauseEventListenersForTarget(gMenuLayer, true);
    darkLayer->setVisible(true);
}
void
NotificationLayer::resumeEventListener()
{
    _eventDispatcher->resumeEventListenersForTarget(gGameLayer, true);
    _eventDispatcher->resumeEventListenersForTarget(gMenuLayer, true);
    darkLayer->setVisible(false);
}
void
NotificationLayer::pauseEvent(cocos2d::Node* ndoe)
{
    auto dispatcher = this->getEventDispatcher();
    dispatcher->pauseEventListenersForTarget(ndoe, true);
}

void
NotificationLayer::resumeEvent(cocos2d::Node * node)
{
    auto dispatcher = this->getEventDispatcher();
    dispatcher->resumeEventListenersForTarget(node, true);
}
/*
void
NotificationLayer::confirm()
{
    pauseEventListener();//暂停下层的事件
    
    auto confirm = CSLoader::createNode(kConfirmLayer);
    confirm->setPosition(gCenter);
    confirm->setScale(.5f);

    ui::Button *button = static_cast<ui::Button*>(confirm->getChildByName("confirmBtn"));
    button->addClickEventListener(std::bind([=](NotificationLayer* layer) {
        layer->resumeEventListener();//恢复下层的事件
    }, this));
    
    auto scale = ScaleTo::create(.5f, 1.0f);
    auto move_ease_in = EaseBounceOut::create(scale->clone());
    auto seq1 = Sequence::create(move_ease_in, nullptr);
    confirm->runAction(seq1);
}
*/
void
NotificationLayer::loading()
{
    loadSprite->setVisible(true);
    loadSprite->runAction(loadAction);
    pauseEventListener();
}
void
NotificationLayer::quitLoading()
{
    loadSprite->setVisible(false);
    loadSprite->stopAllActions();
    resumeEventListener();
}


bool errDelay(false);
std::deque<std::string> errors;
void 
NotificationLayer::onError(const std::string& str)
{
    auto label = Label::createWithSystemFont(str.c_str(), "", 24);
    label->setPosition(Vec2(gCenter.x, gCenter.y*.5));
	this->addChild(label);
    log("system font:%s", label->getSystemFontName().c_str());

	auto move = MoveBy::create(1.0f, Vec2(0, gCenter.y*.5f));
    auto move_ease_in = EaseExponentialIn::create(move->clone());
	auto move_ease_out = EaseExponentialOut::create(move->clone());
	auto action = Sequence::create(
		move_ease_out,
		DelayTime::create(1.5f),
                                   /*队列延迟播放
        CallFunc::create(std::bind([=](NotificationLayer* n){
        errDelay = false;
        if(errors.size())
        {
            std::string err=errors.front();
            errors.pop_front();
            n->onError(err);
        }
        },this)),*/
		move_ease_in,
		RemoveSelf::create(),
		nullptr);
	label->runAction(action);
}


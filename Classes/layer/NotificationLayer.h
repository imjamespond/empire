#pragma once

#include <string>
#include "cocos2d.h"

class NotificationLayer;
extern NotificationLayer *gNotificationLayer;

class NotificationLayer : public cocos2d::Layer
{
public:
	NotificationLayer();
	~NotificationLayer();

	CREATE_FUNC(NotificationLayer);

	virtual bool init();//to invoke method of subclass
	virtual void update(float);
    
    void pauseEventListener();
    void resumeEventListener();
    void pauseEvent(cocos2d::Node*);
    void resumeEvent(cocos2d::Node*);
    
    void autoReconnect();
    
    void pauseTimer();

	void onError(const std::string&);
    //void confirm();
    void loading();
    void quitLoading();
    
    cocos2d::Label *debug;
    cocos2d::Layer *darkLayer;
    cocos2d::Sprite *loadSprite;
    cocos2d::RepeatForever *loadAction;

private:
    void autoReconnectLayer(float);
};
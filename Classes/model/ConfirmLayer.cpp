//
//  ConfirmLayer.cpp
//  empire
//
//  Created by metasoft on 16/7/4.
//
//

#include "ConfirmLayer.hpp"
#include "../layer/NotificationLayer.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace codechiev;

bool
ConfirmLayer::init()
{
    if(!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kConfirmLayer)));
    
    auto confirm = static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Confirm")) ;
    confirm->addClickEventListener(std::bind(&ConfirmLayer::confirm, this, std::placeholders::_1));
    
    text = static_cast<ui::Text*>(modalLayer->getChildByName("TX_Confirm"));
    
    return true;
}

void
ConfirmLayer::confirm(cocos2d::Ref *ref)
{
    if(cb)
    {
        cb();
        cb = confirm_callback();
        this->onClose(nullptr);
    }
}

void
ConfirmLayer::undoModal()
{
    if(target)
    {
        resumeEvent(target);
    }
    
    ModalLayer::undoModal();
}

void
ConfirmLayer::pauseEvent(cocos2d::Node* ndoe)
{
    auto dispatcher = gNotificationLayer->getEventDispatcher();
    dispatcher->pauseEventListenersForTarget(ndoe, true);
    target=ndoe;
}

void
ConfirmLayer::resumeEvent(cocos2d::Node * node)
{
    auto dispatcher = gNotificationLayer->getEventDispatcher();
    dispatcher->resumeEventListenersForTarget(node, true);
    target=nullptr;
}
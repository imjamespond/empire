//
//  ModalLayer.cpp
//  empire
//
//  Created by metasoft on 16/6/17.
//
//

#include "ModalLayer.hpp"
#include "NavigateLayer.hpp"
#include "../layer/NotificationLayer.h"
#include "../layer/MenuLayer.h"
#include "../codechiev/BasisUtil.h"

using namespace codechiev;
USING_NS_CC;

ModalLayer::modal_vec ModalLayer::modals;

bool
ModalLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 200));
    darkLayer->setScale(2.0f);
    this->addChild(darkLayer,-1);
    
    modalLayer=nullptr;
    return true;
}
void
ModalLayer::initModal(cocos2d::Layer* modal)
{
    assert(modal);
    modalLayer = modal;
    modalLayer->setPosition(gCenter);
    this->addChild(modalLayer);
    
    auto close=static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Close"));
    close->addClickEventListener(std::bind(&ModalLayer::onClose, this, std::placeholders::_1));
}
void
ModalLayer::doModal()
{
    if(modalLayer)
        showElasticDialog(this);
    auto disp = gNotificationLayer->getEventDispatcher();
    disp->resumeEventListenersForTarget(this);
    disp->pauseEventListenersForTarget(gMenuLayer->navLayer, true);
}
void
ModalLayer::undoModal()
{
    this->setVisible(false);
    auto disp = gNotificationLayer->getEventDispatcher();
    disp->pauseEventListenersForTarget(this);
    disp->resumeEventListenersForTarget(gMenuLayer->navLayer, true);
    
    this->removeFromParent();
}
void
ModalLayer::onClose(Ref*)
{
    undoModal();
}

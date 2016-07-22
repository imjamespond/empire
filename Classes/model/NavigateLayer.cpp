//
//  MainUI.cpp
//  empire
//
//  Created by metasoft on 16/4/21.
//
//

#include "NavigateLayer.hpp"
#include "../layer/MenuLayer.h"
#include "../layer/GameLayer.h"
#include "../codechiev/BasisUtil.h"
#include "../resource.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace codechiev;

NavigateLayer::NavigateLayer():cursor(nullptr),radioGroup(nullptr),pageView(nullptr)
{}

bool
NavigateLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    return true;
}

void
NavigateLayer::initNav(MenuLayer* menu)
{
    static const int NUMBER_OF_PAGES = 3;
    static const int DEFAULT_PAGE = 1;
    
    // Create the page view
    pageView = NavPageView::create();
    pageView->setContentSize(Size(gVisibleSize));
    pageView->removeAllItems();
    pageView->mainui = this;
    
    for (int i = 0; i < NUMBER_OF_PAGES; ++i)
    {
        ui::Layout* layout = ui::Layout::create();
        //layout->setContentSize(Size(gCenter * .5f));
        pageView->insertCustomItem(layout, i);
    }
    
    pageView->setCurrentPageIndex(DEFAULT_PAGE);
    this->addChild(pageView);
    
    //nav menu ui
    auto navmenuLayer = CSLoader::createNode(kNavMenuLayer);
    navmenuLayer->setPositionX(gCenter.x);
    this->addChild(navmenuLayer);
    
    cursor = static_cast<Sprite*>(navmenuLayer->getChildByName("Sprite_Cursor")) ;
    
    radioGroup = ui::RadioButtonGroup::create();
    radioGroup->addEventListener(CC_CALLBACK_3(NavigateLayer::onRadioChange, this));
    this->addChild(radioGroup);
    
    ui::RadioButton* radioButton = nullptr;
    radioButton = ui::RadioButton::create(kImgNavAUnsel, kImgNavA);
    navmenuLayer->getChildByName("Node_3")->addChild(radioButton);
    radioGroup->addRadioButton(radioButton);
    radioButton = ui::RadioButton::create(kImgNavBUnsel, kImgNavB);
    navmenuLayer->getChildByName("Node_2")->addChild(radioButton);
    radioGroup->addRadioButton(radioButton);
    radioButton = ui::RadioButton::create(kImgNavCUnsel, kImgNavC);
    navmenuLayer->getChildByName("Node_1")->addChild(radioButton);
    radioGroup->addRadioButton(radioButton);
    radioGroup->setSelectedButton(DEFAULT_PAGE);
    
    ui::RadioButton* dfBtn = radioGroup->getRadioButtonByIndex(DEFAULT_PAGE);
    cursor->setPosition(dfBtn->getPosition());
}

void
NavigateLayer::onRadioChange(ui::RadioButton* radioButton, int index, ui::RadioButtonGroup::EventType type)
{
    if(pageView)
        pageView->scrollToItem(index);
}

NavPageView::NavPageView():mainui(nullptr){}

void
NavPageView::moveInnerContainer(const cocos2d::Vec2 &deltaMove, bool canStartBounceBack)
{
    PageView::moveInnerContainer(deltaMove, canStartBounceBack);
    
    if(mainui->cursor)
    {
        mainui->cursor->setPositionX(mainui->cursor->getPositionX()-deltaMove.x*mainui->cursor->getContentSize().width/this->getContentSize().width);
    }

    if(canStartBounceBack)
    {
        
    }
    
}

void
NavPageView::handleReleaseLogic(cocos2d::Touch *touch)
{
    PageView::handleReleaseLogic(touch);
    
    if(mainui->radioGroup)
        mainui->radioGroup->setSelectedButton(this->getCurrentPageIndex());
}

//
//  CycleShowPageView.cpp
//  empire
//
//  Created by metasoft on 16/6/14.
//
//

#include "CycleShowPageView.hpp"

using namespace cocos2d;
using namespace codechiev;

void pageViewEvent(cocos2d::Ref *pSender, cocos2d::ui::PageView::EventType type);

bool
CycleShowPageView::init()
{
    if(!ui::PageView::init())
    {
        return false;
    }
    
    this->addEventListener(std::bind(&pageViewEvent,std::placeholders::_1,std::placeholders::_2));
    
    return true;
}

void
CycleShowPageView::moveInnerContainer(const cocos2d::Vec2 &deltaMove, bool canStartBounceBack)
{
    PageView::moveInnerContainer(deltaMove, canStartBounceBack);
    
    //log(this->getCurrentPageIndex(), deltaMove.x, canStartBounceBack);
    if(canStartBounceBack && this->getCurrentPageIndex()==this->getItems().size()-1)
    {
        auto page = this->getItem(0);
        this->removePageAtIndex(0);
        this->addPage(page);
        this->setCurrentPageIndex(this->getItems().size()-2);
    }
}

void
pageViewEvent(Ref *pSender, ui::PageView::EventType type)
{
    //base::Singleton<BaseDebug>::get()->add("pageViewEvent");
    switch (type)
    {
        case ui::PageView::EventType::TURNING:
        {
            //CycleShowPageView* pageView = dynamic_cast<CycleShowPageView*>(pSender);
            //log("pageViewEvent: %zd", pageView->getCurrentPageIndex());
        }
            break;
            
        default:
            break;
    }
}
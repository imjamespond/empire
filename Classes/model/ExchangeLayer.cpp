//
//  ExchangeLayer.cpp
//  empire
//
//  Created by metasoft on 16/6/30.
//
//

#include "ExchangeLayer.hpp"
#include "CardLayer.hpp"
#include "ActionSprite.h"
#include "RecieveOkLayer.hpp"
#include "../layer/MenuLayer.h"
#include "../layer/NotificationLayer.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/control/LoginController.h"
#include <cocostudio/CocoStudio.h>

using namespace cocos2d;
using namespace codechiev;

ExchangeLayer::ExchangeLayer(){}
int scores[]={300,500,800,1000,1500};
bool
ExchangeLayer::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kExchangeLayer)));
    
    for(int i=0; i<(sizeof scores)/sizeof(int); i++)
    {
        ui::Button* btn = static_cast<ui::Button*>(this->modalLayer->getChildByName(StringUtils::format("Btn%d", scores[i])));
        btn->addClickEventListener(std::bind(&ExchangeLayer::exchange, this, scores[i]));
    }
        
    return true;
}

void
ExchangeLayer::exchange(int score)
{
    control::LoginController::exchange(0, CmdCallbackBeginWithData
                                       if(ok){
                                           gNotificationLayer->onError("兑换成功");
                                           RecieveOkLayer::show(gMenuLayer, data);
                                       }
                                       CmdCallbackEnd);
}

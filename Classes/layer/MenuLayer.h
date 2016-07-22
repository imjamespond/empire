#pragma once

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <external/json/document.h>
#include "../model/Game.hpp"

class MenuLayer;
extern MenuLayer *gMenuLayer;

namespace codechiev {
    class HallLayer;
    class IntroLayer;
    class RankLayer;
    class RedeemLayer;
    class CardLayer;
    class CardDescLayer;
    class MessageLayer;
    class NavigateLayer;
    class Configure;
    class PvpSearchLayer;
    class UpgradeOkLayer;
    class ExchangeLayer;
    class ConfirmLayer;
}

class MenuLayer : public cocos2d::Layer
{
public:
	MenuLayer();
	~MenuLayer();

	CREATE_FUNC(MenuLayer);

	virtual bool init();//to invoke method of subclass
	virtual void onEnter();
	virtual void update(float);

	// a selector callback
    void gameEvent(rapidjson::Document& );
    void gameEndEvent(rapidjson::Document& );
    void updateEvent(rapidjson::Document& );
    
    codechiev::HallLayer *hallLayer;
    codechiev::MessageLayer *msgLayer;
    codechiev::CardLayer *cardLayer;
    codechiev::NavigateLayer* navLayer;
    
    //codechiev::RedeemLayer *redeemLayer;
    //codechiev::Configure* configLayer;
    //codechiev::RankLayer *rankLayer;
    //codechiev::CardDescLayer *cardDescLayer;
    //codechiev::IntroLayer *introLayer;
    codechiev::PvpSearchLayer *pvpSearchLayer;
    //codechiev::ExchangeLayer *exchangeLayer;
    //codechiev::ConfirmLayer *confirmLayer;

private:
    void testRichText(cocos2d::ui::Widget*);
    
    codechiev::Game *game_;
};
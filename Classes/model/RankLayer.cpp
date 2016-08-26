//
//  RankLayer.cpp
//  empire
//
//  Created by metasoft on 16/6/17.
//
//

#include "RankLayer.hpp"
#include "../resource.h"
#include "../codechiev/BasisUtil.h"
#include "../layer/GameLayer.h"
#include "../layer/MenuLayer.h"
#include "../codechiev/control/LoginController.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace codechiev;

RankLayer::RankLayer(){}

bool
RankLayer::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kRankLayer)));
    
    //radio group
    auto posnode = CSLoader::createNode(kRankPosLayer);
    auto score = posnode->getChildByName("Score");
    auto redeem = posnode->getChildByName("Redeem");
    ui::RadioButtonGroup *radioGroup = ui::RadioButtonGroup::create();
    addChild(radioGroup);
    radioGroup->setAllowedNoSelection(false);
    radioGroup->addEventListener(CC_CALLBACK_3(RankLayer::onRadioChange, this));
    
    ui::RadioButton* radioScore = ui::RadioButton::create(kUIScoreTab, kUIScoreTabSel);
    radioScore->setPosition(score->getPosition()+gCenter);
    this->addChild(radioScore);
    radioGroup->addRadioButton(radioScore);
    ui::RadioButton* radioRedeem = ui::RadioButton::create(kUIRedeemTab, kUIRedeemTabSel);
    radioRedeem->setPosition(redeem->getPosition()+gCenter);
    this->addChild(radioRedeem);
    radioGroup->addRadioButton(radioRedeem);
    
    rankScore();
    return true;
}

void
RankLayer::onClose(cocos2d::Ref *ref)
{
    auto rankList = static_cast<ui::ListView*>(modalLayer->getChildByName("ListView_1"));
    rankList->removeAllItems();
    log("rank layer close");
    
    ModalLayer::onClose(ref);
}

int gIndex(0);
void
RankLayer::onRadioChange(ui::RadioButton* radioButton, int index, ui::RadioButtonGroup::EventType type)
{
    if(gIndex==index)
        return;
    gIndex = index;
    
    auto rankList = static_cast<ui::ListView*>(modalLayer->getChildByName("ListView_1"));
    rankList->removeAllItems();
    
    if(index==0)
    {
        rankScore();
    }else
    {
        rankRedeem();
    }
}

void
RankLayer::rankScore()
{
    codechiev::control::LoginController::rankList(0, 20, std::bind([=](RankLayer* rankLayer, rapidjson::Document& doc, bool ok){
        const rapidjson::Value& data = doc["data"];
        if(ok)
        {
            auto rankList = static_cast<ui::ListView*>(rankLayer->modalLayer->getChildByName("ListView_1"));
            assert(!data.IsNull() && data.IsArray());
            for(rapidjson::SizeType i=0; i<data.Size(); i++)
            {
                const rapidjson::Value &val = data[i];
                auto rankNode = CSLoader::createNode(kRankNode);
                auto num = static_cast<ui::Text*>(rankNode->getChildByName("Text_Num")) ;
                auto numsprite = static_cast<Sprite*>(rankNode->getChildByName("Sprite_Num")) ;
                auto name = static_cast<ui::Text*>(rankNode->getChildByName("Text_Name")) ;
                auto score = static_cast<ui::Text*>(rankNode->getChildByName("Text_Score")) ;
                auto level = static_cast<ui::Text*>(rankNode->getChildByName("Text_Level")) ;
                auto sprite = static_cast<Sprite*>(rankNode->getChildByName("Sprite_1")) ;
                num->setString(StringUtils::format("%d", i+1));
                if(i<3)
                {
                    numsprite->setVisible(true);
                    numsprite->initWithFile(StringUtils::format(kImgRankNum, i) );
                }
                name->setString(val["name"].GetString());
                const rapidjson::Value &scoreInt = val["score"];
                const rapidjson::Value &lvInt = val["level"];
                assert(!scoreInt.IsNull() && scoreInt.IsInt());
                score->setString(StringUtils::format("%d", scoreInt.GetInt()));
                level->setString(StringUtils::format("LV.%d", lvInt.GetInt()));
                auto layout = ui::Layout::create();
                layout->setContentSize(sprite->getContentSize());
                layout->addChild(rankNode);
                rankNode->setPosition(Vec2(sprite->getContentSize()*.5f));
                rankList->pushBackCustomItem(layout);
            }
        }
    }, this, std::placeholders::_1, std::placeholders::_2));
}

void
RankLayer::rankRedeem()
{
    codechiev::control::LoginController::rankRedeem(0, 20, std::bind([=](RankLayer* rankLayer, rapidjson::Document& doc, bool ok){
        const rapidjson::Value& data = doc["data"];
        if(ok)
        {
            auto rankList = static_cast<ui::ListView*>(rankLayer->modalLayer->getChildByName("ListView_1"));
            assert(!data.IsNull() && data.IsArray());
            for(rapidjson::SizeType i=0; i<data.Size(); i++)
            {
                const rapidjson::Value &val = data[i];
                auto rankNode = CSLoader::createNode(kRankNode);
                auto num = static_cast<ui::Text*>(rankNode->getChildByName("Text_Num")) ;
                auto numsprite = static_cast<Sprite*>(rankNode->getChildByName("Sprite_Num")) ;
                auto name = static_cast<ui::Text*>(rankNode->getChildByName("Text_Name")) ;
                auto score = static_cast<ui::Text*>(rankNode->getChildByName("Text_Score")) ;
                auto level = static_cast<ui::Text*>(rankNode->getChildByName("Text_Level")) ;
                auto sprite = static_cast<Sprite*>(rankNode->getChildByName("Sprite_1")) ;
                num->setString(StringUtils::format("%d", i+1));
                if(i<3)
                {
                    numsprite->setVisible(true);
                    numsprite->initWithFile(StringUtils::format(kImgRankNum, i) );
                }
                name->setString(val["name"].GetString());
                const rapidjson::Value &scoreInt = val["score"];
                const rapidjson::Value &lvInt = val["level"];
                assert(!scoreInt.IsNull() && scoreInt.IsInt());
                score->setString(StringUtils::format("%d", scoreInt.GetInt()));
                level->setString(StringUtils::format("LV.%d", lvInt.GetInt()));
                auto layout = ui::Layout::create();
                layout->setContentSize(sprite->getContentSize());
                layout->addChild(rankNode);
                rankNode->setPosition(Vec2(sprite->getContentSize()*.5f));
                rankList->pushBackCustomItem(layout);
            }
        }
    }, this, std::placeholders::_1, std::placeholders::_2));
}

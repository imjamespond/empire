//
//  RecieveOkLayer.cpp
//  empire
//
//  Created by metasoft on 16/7/1.
//
//

#include "RecieveOkLayer.hpp"
#include "CardLayer.hpp"
#include "ActionSprite.h"
#include "NavigateLayer.hpp"
#include "../layer/MenuLayer.h"
#include "../codechiev/BasisUtil.h"
#include <cocostudio/CocoStudio.h>

using namespace cocos2d;
using namespace codechiev;

RecieveOkLayer::RecieveOkLayer():curFrame(nullptr){}

void
initRecievOkFrame(CardFrame *, Node *, const CardStruct& , const RoleStruct& );
bool
RecieveOkLayer::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    
    return true;
}

void
RecieveOkLayer::show(Node* parent, const rapidjson::Value& data)
{
    auto layer = RecieveOkLayer::create();
    parent->addChild(layer);
    
    layer->initModal( static_cast<Layer*>(CSLoader::createNode(kRecieveOkLayer)));
    layer->name = static_cast<ui::Text*>(layer->modalLayer->getChildByName("Text_Name")) ;
    
    auto posLayer = CSLoader::createNode(kRecieveOkPosLayer);
    CardConfig *cc = base::Singleton<CardConfig>::get();
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = std::bind([=](Touch* touch, Event* event){
        return true;
    }, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchEnded = std::bind([=](Touch* touch, Event* event){
        auto target = static_cast<RecieveOkLayer*>(event->getCurrentTarget()) ;
        target->hide();
    }, std::placeholders::_1, std::placeholders::_2);
    auto dispatcher = layer->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
    
    for(rapidjson::SizeType i=0; i<data.Size(); i++)
    {
        const rapidjson::Value &val = data[i];
        const rapidjson::Value& roleid = val["id"];
        const rapidjson::Value& level = val["level"];
        const rapidjson::Value& amount = val["amount"];
        
        CardConfig::card_struct_map::const_iterator it = cc->cardMap.find(roleid.GetInt());
        //card icon data
        const CardStruct& card = it->second;
        //init properties
        AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(card.id);
        if(roleIt!=ac->roleStructMap.end())
        {
            
            const RoleStruct& role = roleIt->second;
            auto frame = CardFrame::create();
            frame->retain();
            frame->setAnchorPoint(Vec2::ZERO);
            frame->setPosition(gCenter);
            initRecievOkFrame(frame , posLayer, card, role);
            frame->id=roleid.GetInt();
            
            CardUnit** cards=gMenuLayer->cardLayer->cardsel;
            for(int i=0;  i< (sizeof gMenuLayer->cardLayer->cardsel)/sizeof(CardUnit*); i++)
            {
                if(frame->id==cards[i]->frame->id)
                    frame->operator=(cards[i]->frame);
            }
            frame->level=level.GetInt();
            frame->number=amount.GetInt();
            frame->setUpgrade();
            layer->frames.push_back(frame);
            
            CardFrame* cf = gMenuLayer->cardLayer->getCardFrame(roleid.GetInt());
            if(cf)
            {
                cf->number = amount.GetInt();
                cf->level = level.GetInt();
                cf->setUpgrade();
            }
        }
    }
    
    layer->hide();
}

void
RecieveOkLayer::hide()
{
    if(curFrame)
    {
        this->removeChild(curFrame);
        curFrame->release();
    }
    if(frames.size())
    {
        curFrame = frames.front();
        this->addChild(curFrame);
        this->name->setString(getRoleName(curFrame->id));
        frames.pop_front();
    }
    else
    {
        this->removeFromParent();
    }

}

void
initRecievOkFrame(CardFrame *frame, Node *posNode, const CardStruct& card, const RoleStruct& role)
{
    frame->id = role.id;
    frame->type = role.type;
    
    auto cardFrame = static_cast<Sprite*>(posNode->getChildByName("card_frame"));
    auto cardFrameMask = static_cast<Sprite*>(posNode->getChildByName("card_frame_mask"));
    auto upgrade = static_cast<Sprite*>(posNode->getChildByName("Text_Upgrade"));
    auto glow = static_cast<Sprite*>(posNode->getChildByName("Sprite_Glow"));
    auto expbar = static_cast<Sprite*>(posNode->getChildByName("expbar"));
    const Size &expbarSize = expbar->getContentSize();
    //card image
    frame->spriteFrame = Sprite::createWithSpriteFrameName(card.src.c_str());
    frame->spriteFrame->setScale(3);
    frame->spriteFrame->setPosition(cardFrameMask->getPosition());
    frame->clip->addChild(frame->spriteFrame);
    //card frame mask
    auto cfmask = Sprite::createWithTexture(cardFrameMask->getTexture());
    cfmask->setPosition(cardFrameMask->getPosition());
    frame->clip->setStencil(cfmask);
    //card frame
    frame->cardFrame = Sprite::createWithTexture(cardFrame->getTexture());
    frame->cardFrame->setPosition(cardFrame->getPosition());
    frame->setContentSize(frame->cardFrame->getContentSize());
    frame->addChild(frame->cardFrame);
    //expbar
    frame->expbar = ui::Scale9Sprite::create();
    frame->expbar->init(Sprite::createWithTexture(expbar->getTexture()) ,
                        Rect(0, 0, expbarSize.width, expbarSize.height),
                        Rect(4, 4, expbarSize.width-8, expbarSize.height-8));
    frame->expbar->setAnchorPoint(Vec2::ZERO);
    frame->expbar->setPosition(expbar->getPosition());
    frame->expbarWidth = expbarSize.width;
    frame->addChild(frame->expbar);
    //number
    frame->upgradeText = ui::Text::create();
    frame->upgradeText->setPosition(upgrade->getPosition());
    frame->addChild(frame->upgradeText);
    //glow
    frame->cardGlow->initWithFile(StringUtils::format("img/type%dlarge.png", role.type));
    frame->cardGlow->setPosition(glow->getPosition());
    frame->setAnchorPoint(Vec2::ZERO);
}
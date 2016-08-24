//
//  RedeemLayer.cpp
//  empire
//
//  Created by metasoft on 16/6/28.
//
//

#include "RedeemLayer.hpp"
#include "CardLayer.hpp"
#include "ActionSprite.h"
#include "ConfirmLayer.hpp"
#include "User.h"
#include "../layer/MenuLayer.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/control/LoginController.h"
#include <cocostudio/CocoStudio.h>

using namespace cocos2d;
using namespace codechiev;

RedeemLayer::RedeemLayer(){}

void
initRedeemFrame(CardFrame *, Node *, const CardStruct& , const RoleStruct& );
bool
RedeemLayer::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kRedeemLayer)));
    auto redeem = static_cast<ui::Text *>(this->modalLayer->getChildByName("Text_Redeem"));
    
    User *self = base::Singleton<User>::get();
    redeem->setString(StringUtils::format("%d",self->redeem));
    
    //init every card
    int i(0);
    auto posLayer = CSLoader::createNode(kRedeemPosLayer);
    auto posNode = posLayer->getChildByName("Node_Frame");
    CardConfig *cc = base::Singleton<CardConfig>::get();
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    for(CardConfig::card_struct_map::iterator it = cc->cardMap.begin();
        it != cc->cardMap.end();
        it++)
    {
        Node *node = posLayer->getChildByName(StringUtils::format("Frame_%d",i));
        //card icon data
        CardStruct& card = it->second;
        //init properties
        AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(card.id);
        if(roleIt!=ac->roleStructMap.end())
        {
            const RoleStruct& role = roleIt->second;
            auto frame = CardFrame::create();
            frame->setAnchorPoint(Vec2::ZERO);
            frame->setPosition(node->getPosition()+gCenter);
            this->addChild(frame);
            initRedeemFrame(frame , posNode, card, role);
            frames[frame->id]=frame;
        }
        
        i++;
    }
    
    //add touch event listener
    //auto listener = EventListenerTouchAllAtOnce::create();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = std::bind(&RedeemLayer::touchbegin, this,
                                       std::placeholders::_1, std::placeholders::_2);
    for(frame_map::const_iterator it=frames.begin();
        it!=frames.end();
        it++)
    {
        CardFrame* f = it->second;
        f->listener = listener->clone();
        f->getListener()->setEnabled(false);
        auto disp = this->getEventDispatcher();
        disp->addEventListenerWithSceneGraphPriority(f->getListener(), f);
    }
    
    return true;
}
void
RedeemLayer::updateFrame(codechiev::CardFrame *cf)
{
    frame_map::iterator it = frames.find(cf->id);
    if(it!=frames.end())
    {
        CardFrame* f = it->second;
        f->operator=(cf);
        f->upgradeText->setString(StringUtils::format("%d", f->number));
    }
    
}
bool
RedeemLayer::touchbegin(Touch* touch, Event* event)
{
    auto target = static_cast<CardFrame*>(event->getCurrentTarget());
    
    Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    //check is selected
    if (rect.containsPoint(locationInNode))
    {
        auto confirmLayer = ConfirmLayer::create();
        confirmLayer->setVisible(false);
        gMenuLayer->addChild(confirmLayer,999);
        confirmLayer->cb = boost::bind(&control::LoginController::redeem, target->id,
            std::bind([=](ConfirmLayer* confirmLayer, rapidjson::Document& doc, bool ok){
        const rapidjson::Value& data = doc["data"];
        if(ok&&!data.IsNull())
        {
           gMenuLayer->cardLayer->updateCard(data);
           confirmLayer->resumeEvent(this);
        }
        }, confirmLayer, std::placeholders::_1, std::placeholders::_2));
        confirmLayer->pauseEvent(this);
        confirmLayer->doModal();
        confirmLayer->text->setString(StringUtils::format("确定要将%s赎罪么?", getRoleName(target->id).c_str()));
        
        return true;
    }
    return false;
}
void
RedeemLayer::doModal()
{
    for(frame_map::const_iterator it=frames.begin();
        it!=frames.end();
        it++)
    {
        CardFrame* f = it->second;
        f->getListener()->setEnabled(true);
    }
    
    CardUnit **cards = gMenuLayer->cardLayer->cardsel;
    for(int i=0; i<(sizeof gMenuLayer->cardLayer->cardsel)/sizeof(CardUnit*); i++)
    {
        updateFrame(cards[i]->frame);
    }
    
    ModalLayer::doModal();
}
void
RedeemLayer::undoModal()
{
    for(frame_map::const_iterator it=frames.begin();
        it!=frames.end();
        it++)
    {
        CardFrame* f = it->second;
        f->getListener()->setEnabled(false);
    }
    
    ModalLayer::undoModal();
}

void
initRedeemFrame(CardFrame *frame, Node *posNode, const CardStruct& card, const RoleStruct& role)
{
    frame->id = role.id;
    frame->type = role.type;
    
    auto cardFrame = static_cast<Sprite*>(posNode->getChildByName("card_frame"));
    auto cardFrameMask = static_cast<Sprite*>(posNode->getChildByName("card_frame_mask"));
    auto number = static_cast<Sprite*>(posNode->getChildByName("Text_Number"));
    auto glow = static_cast<Sprite*>(posNode->getChildByName("Sprite_Glow"));

    //card image
    frame->spriteFrame = Sprite::createWithSpriteFrameName(card.src.c_str());
    //frame->spriteFrame->setScale(3);
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

    //number
    frame->upgradeText = ui::Text::create();
    frame->upgradeText->setPosition(number->getPosition());
    frame->upgradeText->setTextColor(Color4B(0xff,0xff,0xff,255));
    frame->upgradeText->setFontSize(20);
    frame->addChild(frame->upgradeText);
    //glow
    frame->cardGlow->initWithFile(StringUtils::format("img/type%d.png", role.type));
    frame->cardGlow->setPosition(glow->getPosition());
    frame->setAnchorPoint(Vec2::ZERO);
    //type icon
    frame->spriteType = Sprite::create("img/redeem_type.png");
    frame->spriteType->setPosition(cardFrame->getPosition());
    frame->addChild(frame->spriteType);
}
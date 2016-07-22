//
//  UpgradeOkLayer.cpp
//  empire
//
//  Created by metasoft on 16/6/30.
//
//

#include "UpgradeOkLayer.hpp"
#include "CardLayer.hpp"
#include "ActionSprite.h"
#include "NavigateLayer.hpp"
#include "../layer/MenuLayer.h"
#include "../codechiev/BasisUtil.h"
#include <cocostudio/CocoStudio.h>

using namespace cocos2d;
using namespace codechiev;

UpgradeOkLayer::UpgradeOkLayer(){}

void
initUpgradeOkFrame(CardFrame *, Node *, const CardStruct& , const RoleStruct& );
bool
UpgradeOkLayer::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    
    return true;
}

void
UpgradeOkLayer::show(Node* parent, const rapidjson::Value& data)
{
    auto upgradeOkLayer = UpgradeOkLayer::create();
    parent->addChild(upgradeOkLayer);
    
    upgradeOkLayer->initModal( static_cast<Layer*>(CSLoader::createNode(kUpgradeOkLayer)));
    upgradeOkLayer->name = static_cast<ui::Text*>(upgradeOkLayer->modalLayer->getChildByName("Text_Name")) ;
    auto node = upgradeOkLayer->modalLayer->getChildByName("Node_Info");
    upgradeOkLayer->attack = static_cast<ui::Text*>(node->getChildByName("Text_Attack")) ;
    upgradeOkLayer->hp = static_cast<ui::Text*>(node->getChildByName("Text_Hp")) ;
    
    auto posLayer = CSLoader::createNode(kUpgradeOkPosLayer);
    CardConfig *cc = base::Singleton<CardConfig>::get();
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    CardConfig::card_struct_map::const_iterator it = cc->cardMap.find(1);
    //card icon data
    const CardStruct& card = it->second;
    upgradeOkLayer->frame = CardFrame::create();
    
    //init properties
    AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(card.id);
    if(roleIt!=ac->roleStructMap.end())
    {
        const RoleStruct& role = roleIt->second;
        upgradeOkLayer->frame = CardFrame::create();
        upgradeOkLayer->frame->setPosition(gCenter);
        initUpgradeOkFrame(upgradeOkLayer->frame , posLayer, card, role);
    }
    
    upgradeOkLayer->frame->setAnchorPoint(Vec2::ZERO);
    upgradeOkLayer->addChild(upgradeOkLayer->frame);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = std::bind([=](Node* parent, Touch* touch, Event* event){
        auto target = static_cast<UpgradeOkLayer*>(event->getCurrentTarget()) ;
        target->undoModal();
        auto desc = static_cast<CardDescLayer*>(parent);
        desc->undoModal();
        return true;
    }, parent, std::placeholders::_1, std::placeholders::_2);
    auto dispatcher = upgradeOkLayer->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, upgradeOkLayer);
    
    CardFrame* cf = gMenuLayer->cardLayer->updateCard(data);
    if(cf)
        upgradeOkLayer->updateFrame(cf);
}

void
UpgradeOkLayer::updateFrame(const CardFrame *cf)
{
    name->setString(getRoleName(cf->id));
    attack->setString(StringUtils::format("%d",cf->attack));
    hp->setString(StringUtils::format("%d",cf->hp));
    frame->id = cf->id;
    frame->operator=(cf);
    frame->changeLarge();
}

void
initUpgradeOkFrame(CardFrame *frame, Node *posNode, const CardStruct& card, const RoleStruct& role)
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
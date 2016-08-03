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

#define ROT_STEP 5
Node* n1(nullptr);
Node* n2(nullptr);
RecieveOkLayer* gRecieveOkLayer;
void
RecieveOkLayer::show(Node* parent, const rapidjson::Value& data)
{
    gRecieveOkLayer = RecieveOkLayer::create();
    parent->addChild(gRecieveOkLayer);
    
    gRecieveOkLayer->initModal( static_cast<Layer*>(CSLoader::createNode(kRecieveOkLayer)));
    gRecieveOkLayer->name = static_cast<ui::Text*>(gRecieveOkLayer->modalLayer->getChildByName("TX_11")) ;
    
    auto posLayer = CSLoader::createNode(kRecieveOkPosLayer);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = std::bind([=](Touch* touch, Event* event){
        return true;
    }, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchEnded = std::bind([=](Touch* touch, Event* event){
        auto target = static_cast<RecieveOkLayer*>(event->getCurrentTarget()) ;
        target->hide();
    }, std::placeholders::_1, std::placeholders::_2);
    auto dispatcher = gRecieveOkLayer->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, gRecieveOkLayer);
   
    if(1==data.Size())
    {
        n1 = gRecieveOkLayer->modalLayer->getChildByName("Node_1_1") ;
        //n2 = gRecieveOkLayer->modalLayer->getChildByName("Node_2_2") ;
        n1->setVisible(true);
        //n2->setVisible(true);
        gRecieveOkLayer->schedule([&](float dt) {
            Vec3 v3 = n1->getRotation3D();
            v3.y+=ROT_STEP;
            n1->setRotation3D(v3);
            //n2->setRotation3D(v3);
            if(v3.y>180)
            {
                gRecieveOkLayer->unschedule("update_key");
            }
            else if(v3.y>90)
            {
                auto s1 = static_cast<Sprite*>(n1->getChildByName("Sprite_1_1")) ;
                //auto s2 = static_cast<Sprite*>(n2->getChildByName("Sprite_2_2")) ;
                s1->setVisible(false);
                //s2->setVisible(false);
            }
            
        }, "update_key");
        
        const rapidjson::Value &val1 = data[0];
        const rapidjson::Value& lv1 = val1["level"];
        const rapidjson::Value& am1 = val1["amount"];
        const RoleStruct* role = getRole(val1["id"].GetInt());
        const CardStruct* card = getCard(val1["id"].GetInt());
        auto f1 = CardFrame::create();
        f1->setRotation3D(Vec3(0,-180,0));
        n1->addChild(f1,-1);
        initRecievOkFrame(f1, posLayer, *card, *role);
        
        CardFrame* cf1 = gMenuLayer->cardLayer->getCardFrame(role->id);
        if(cf1)
        {
            cf1->number = am1.GetInt();
            cf1->level = lv1.GetInt();
            cf1->setUpgrade();
        }
    }else if(2==data.Size())
    {
        n1 = gRecieveOkLayer->modalLayer->getChildByName("Node_2_1") ;
        n2 = gRecieveOkLayer->modalLayer->getChildByName("Node_2_2") ;
        n1->setVisible(true);
        n2->setVisible(true);
        gRecieveOkLayer->schedule([&](float dt) {
            Vec3 v3 = n1->getRotation3D();
            v3.y+=ROT_STEP;
            n1->setRotation3D(v3);
            n2->setRotation3D(v3);
            if(v3.y>180)
            {
                gRecieveOkLayer->unschedule("update_key");
            }
            else if(v3.y>90)
            {
                auto s1 = static_cast<Sprite*>(n1->getChildByName("Sprite_2_1")) ;
                auto s2 = static_cast<Sprite*>(n2->getChildByName("Sprite_2_2")) ;
                s1->setVisible(false);
                s2->setVisible(false);
            }
            
        }, "update_key");
        
        const rapidjson::Value &val1 = data[0];
        const rapidjson::Value& lv1 = val1["level"];
        const rapidjson::Value& am1 = val1["amount"];
        const RoleStruct* role = getRole(val1["id"].GetInt());
        const CardStruct* card = getCard(val1["id"].GetInt());
        auto f1 = CardFrame::create();
        f1->setRotation3D(Vec3(0,-180,0));
        n1->addChild(f1,-1);
        initRecievOkFrame(f1, posLayer, *card, *role);
        
        const rapidjson::Value &val2 = data[1];
        const rapidjson::Value& lv2 = val2["level"];
        const rapidjson::Value& am2 = val2["amount"];
        const RoleStruct* role2 = getRole(val2["id"].GetInt());
        const CardStruct* card2 = getCard(val2["id"].GetInt());
        auto f2 = CardFrame::create();
        f2->setRotation3D(Vec3(0,-180,0));
        n2->addChild(f2,-1);
        initRecievOkFrame(f2, posLayer, *card2, *role2);
        
        CardFrame* cf1 = gMenuLayer->cardLayer->getCardFrame(role->id);
        if(cf1)
        {
            cf1->number = am1.GetInt();
            cf1->level = lv1.GetInt();
            cf1->setUpgrade();
        }
        CardFrame* cf2 = gMenuLayer->cardLayer->getCardFrame(role2->id);
        if(cf2)
        {
            cf2->number = am2.GetInt();
            cf2->level = lv2.GetInt();
            cf2->setUpgrade();
        }

    }
    
    /*
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
    
    layer->hide();*/
}
#undef ROT_STEP

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
        gRecieveOkLayer->removeFromParent();
        gRecieveOkLayer=nullptr;
    }

}

void
initRecievOkFrame(CardFrame *frame, Node *posNode, const CardStruct& card, const RoleStruct& role)
{
    frame->id = role.id;
    frame->type = role.type;
    
    auto cardFrame = Sprite::create(StringUtils::format("img/type%dlarge.png", role.type));
    auto cardFrameMask = static_cast<Sprite*>(posNode->getChildByName("card_frame_mask"));

    //card image
    frame->spriteFrame = Sprite::createWithSpriteFrameName(card.src.c_str());
    frame->spriteFrame->setScale(3);
    frame->spriteFrame->setPosition(Vec2::ZERO);
    frame->clip->addChild(frame->spriteFrame);
    //card frame mask
    auto cfmask = Sprite::createWithTexture(cardFrameMask->getTexture());
    cfmask->setPosition(Vec2::ZERO);
    frame->clip->setStencil(cfmask);
    //card frame
    frame->cardFrame = cardFrame;
    frame->cardFrame->setPosition(Vec2::ZERO);
    //frame->setContentSize(frame->cardFrame->getContentSize());
    frame->addChild(frame->cardFrame);

}
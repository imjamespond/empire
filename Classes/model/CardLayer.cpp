//
//  CardLayer.cpp
//  empire
//
//  Created by metasoft on 16/5/9.
//
//

#include "CardLayer.hpp"
#include "ActionSprite.h"
#include "UpgradeOkLayer.hpp"
#include "NavigateLayer.hpp"
#include "../resource.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/base/TimeUtil.h"
#include "../codechiev/control/LoginController.h"
#include "../layer/GameLayer.h"
#include "../layer/MenuLayer.h"
#include "../layer/NotificationLayer.h"
#include <cocostudio/CocoStudio.h>
#include <set>



namespace codechiev
{
    std::string getRoleName(int roleid)
    {
        AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
        AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(roleid);
        if(roleIt!=ac->roleStructMap.end())
        {
            return roleIt->second.name;
        }
        return "";
    }
    const struct RoleStruct* getRole(int roleid)
    {
        AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
        AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(roleid);
        if(roleIt!=ac->roleStructMap.end())
        {
            return &roleIt->second;
        }
        return nullptr;
    }
    
    const struct CardStruct* getCard(int id)
    {
        CardConfig *cc = base::Singleton<CardConfig>::get();
        CardConfig::card_struct_map::const_iterator it = cc->cardMap.find(id);
        //card icon data
        if(it!=cc->cardMap.end())
        {
            return &it->second;
        }
        return nullptr;
    }
}

USING_NS_CC;
using namespace codechiev;

void
initRoleFrame(CardFrame *frame, Node *cardUnitLayer, const CardStruct& card, const RoleStruct& role)
{
    frame->id = role.id;
    frame->type = role.type;
    
    auto cardFrame = static_cast<Sprite*>(cardUnitLayer->getChildByName("card_frame"));
    auto cardFrameMask = static_cast<Sprite*>(cardUnitLayer->getChildByName("card_frame_mask"));
    auto expbar = static_cast<Sprite*>(cardUnitLayer->getChildByName("expbar"));
    auto glow = static_cast<Sprite*>(cardUnitLayer->getChildByName("Sprite_Glow"));
    auto upgrade = static_cast<ui::Text*>(cardUnitLayer->getChildByName("Text_Upgrade"));
    auto level = static_cast<ui::Text*>(cardUnitLayer->getChildByName("Text_Level"));
    
    const Size &expbarSize = expbar->getContentSize();
    
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
    //expbar
    frame->expbar = ui::Scale9Sprite::create();
    frame->expbar->init(Sprite::createWithTexture(expbar->getTexture()) ,
                        Rect(0, 0, expbarSize.width, expbarSize.height),
                        Rect(4, 4, expbarSize.width-8, expbarSize.height-8));
    frame->expbar->setAnchorPoint(Vec2::ZERO);
    frame->expbar->setPosition(expbar->getPosition());
    frame->expbarWidth = expbarSize.width;
    frame->addChild(frame->expbar);
    //upgrade
    frame->upgradeText = ui::Text::create();
    frame->upgradeText->setFontSize(upgrade->getFontSize());
    frame->upgradeText->setPosition(upgrade->getPosition());
    frame->addChild(frame->upgradeText);
    //levelText
    frame->levelText = ui::Text::create();
    frame->levelText->setFontSize(level->getFontSize());
    frame->levelText->setPosition(level->getPosition());
    frame->addChild(frame->levelText);
    //glow
    frame->cardGlow->initWithFile(StringUtils::format("img/type%d.png", role.type));
    frame->cardGlow->setPosition(glow->getPosition());
    frame->setAnchorPoint(Vec2::ZERO);
    //type icon
    frame->spriteType = Sprite::create(StringUtils::format("img/typeicon%d.png", role.type));
    frame->spriteType->setPosition(cardFrame->getPosition());
    frame->addChild(frame->spriteType);
}

CardLayer::CardLayer(){::memset(cardsel, 0, sizeof cardsel);}
CardFrame::CardFrame():id(0),type(0),number(0),upgrade(0),level(0),attack(0),hp(0)
,expbarWidth(0.0f),levelText(nullptr),upgradeText(nullptr),expbar(nullptr),listener(nullptr),
spriteFrame(nullptr),spriteType(nullptr){}
CardUnit::CardUnit(){}

bool
CardFrame::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    //clip node
    clip = ClippingNode::create();
    clip->setAlphaThreshold( 0.1f );//decide pixel is mask or not by of which alpha threshold
    //set content where mask is transparent invisible
    //clip->setStencil(cocos2d::Node *stencil);
    //clip->addChild(cocos2d::Node *content);
    this->addChild(clip);
    
    cardGlow = Sprite::create();
    this->addChild(cardGlow);
    
    return true;
}

void
CardFrame::setUpgrade()
{
    //find current grade
    for(int i=1; i<11; i++)
    {
        int need = 1<<i;
        if(upgrade < need)
        {
            float propotion = number>need?1.0f : static_cast<float>(number)/static_cast<float>(need);
            if(upgradeText)
                upgradeText->setString(StringUtils::format("%d / %d", number, need));
            if(levelText)
                levelText->setString(StringUtils::format("%d", level));
            if(expbar)
                expbar->setPreferredSize(Size(expbarWidth*propotion,
                                          expbar->getContentSize().height));
            if(level)
            {    if(spriteType)
                    spriteType->setVisible(false);
                clip->setVisible(true);
                if(cardGlow)
                    cardGlow->setVisible(true);
                if(cardFrame)
                    cardFrame->setVisible(true);
                if(upgradeText)
                    upgradeText->setVisible(true);
            }else
            {   //spriteFrame->setColor(Color3B(0x33,0x33,0x33));
                if(spriteType)
                    spriteType->setVisible(true);
                clip->setVisible(false);
                if(cardGlow)
                    cardGlow->setVisible(false);
                if(cardFrame)
                    cardFrame->setVisible(false);
                if(upgradeText)
                    upgradeText->setVisible(false);
            }
            break;
        }
    }
}

void
CardFrame::change()
{
    CardConfig *cc = base::Singleton<CardConfig>::get();
    CardConfig::card_struct_map::const_iterator cardIt = cc->cardMap.find(id);
    if(cardIt!=cc->cardMap.end())
    {
        const CardStruct& card = cardIt->second;
        spriteFrame->initWithSpriteFrameName(card.src.c_str());
        auto glow = Sprite::create(StringUtils::format("img/type%d.png", type));
        cardGlow->setTexture(glow->getTexture());
    }
}
void
CardFrame::changeLarge()
{
    CardConfig *cc = base::Singleton<CardConfig>::get();
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    //card icon data
    CardConfig::card_struct_map::const_iterator cardIt = cc->cardMap.find(id);
    AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(id);
    if(cardIt!=cc->cardMap.end() && roleIt!=ac->roleStructMap.end())
    {
        const CardStruct& card = cardIt->second;
        spriteFrame->initWithSpriteFrameName(card.src.c_str());
        auto glow = Sprite::create(StringUtils::format("img/type%dlarge.png", roleIt->second.type));
        cardGlow->setTexture(glow->getTexture());
    }
}

void
CardFrame::operator=(const CardFrame *f)
{
    this->id = f->id;
    if(f->type)
        this->type = f->type;
    this->number = f->number;
    this->upgrade = f->upgrade;
    this->level = f->level;
    this->attack=f->attack;
    this->hp=f->hp;
    change();
    setUpgrade();
}

bool
CardUnit::init()
{
    if(!Sprite::init())
    {
        return false;
    }
   
    return true;
}

bool
CardDescLayer::init()
{
    if(!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kCardDescLayer)));
    
    auto upgradeBtn=static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Upgrade"));
    upgradeBtn->addClickEventListener(std::bind(&CardDescLayer::onUpgrade, this, std::placeholders::_1));
    
    auto posLayer = CSLoader::createNode(kCardDescPosLayer);
    auto nodeFrame = posLayer->getChildByName("Node_Frame");
    
    CardConfig *cc = base::Singleton<CardConfig>::get();
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    //card icon data
    CardConfig::card_struct_map::const_iterator cardIt = cc->cardMap.find(1);
    AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(1);
    if(cardIt!=cc->cardMap.end() && roleIt!=ac->roleStructMap.end())
    {
        const CardStruct& card = cardIt->second;
        const RoleStruct& role = roleIt->second;
        frame = CardFrame::create();
        frame->setAnchorPoint(Vec2::ZERO);
        frame->setPosition(nodeFrame->getPosition()+gCenter);
        this->addChild(frame);
        initRoleFrame(frame , nodeFrame, card, role);
    }
    
    return true;
}
void
CardDescLayer::show( CardFrame* f)
{
    auto cardDescLayer = CardDescLayer::create();
    cardDescLayer->setPosition(Vec2::ZERO);
    gMenuLayer->addChild(cardDescLayer);
    
    cardDescLayer->frame->operator=(f);

    auto txName = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_Name"));
    txName->setString(getRoleName(f->id));
    
    const RoleStruct* role = getRole(f->id);
    auto txDesc = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_Desc"));
    txDesc->setString(role->description);
    auto txProfess = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_Profession"));
    txProfess->setString(role->description);
    auto txSkillName = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_SkillName"));
    txSkillName->setString(role->description);
    auto txSkill = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_Skill"));
    txSkill->setString(role->description);
    
    auto txAttack = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_Attack"));
    txAttack->setString(StringUtils::format("%d", f->attack));
    
    auto txHp = static_cast<ui::Text*>( cardDescLayer->modalLayer->getChildByName("Text_Hp"));
    txHp->setString(StringUtils::format("%d", f->hp));
    
    cardDescLayer->doModal();
}
void
CardDescLayer::onUpgrade(cocos2d::Ref *)
{
    CardUnit **cardsel = gMenuLayer->cardLayer->cardsel;
    CardUnit *card(nullptr);
    //find out which card has been selected
    for(int j=0; j<(sizeof gMenuLayer->cardLayer->cardsel)/sizeof(CardUnit *); j++)
    {
        if(cardsel[j]->frame->id == frame->id)
        {
            card = cardsel[j];
        }
    }
    assert(card);
    
    for(int i=1; i<11; i++)
    {
        int need = 1<<i;
        if(card->frame->upgrade < need)
        {
            if(card->frame->number >= need)
            {
                control::LoginController::upgrade(frame->id, need,
                                                  std::bind([=](CardDescLayer* desc, rapidjson::Document& doc, bool ok){
                    const rapidjson::Value& data = doc["data"];
                    if(ok)
                    {
                        UpgradeOkLayer::show(desc, data);
                    }
                }, this, std::placeholders::_1, std::placeholders::_2));
            }
            else
            {
                gNotificationLayer->onError("您的卡片数量不足!");
            }
            break;
        }
    }
    
}

bool
CardLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }

    auto cardPosLayer = CSLoader::createNode(kCardPosLayer);
    auto cardLayer = CSLoader::createNode(kCardLayer);
    cardLayer->setPosition(gCenter);
    this->addChild(cardLayer);
    
    //add touch event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = std::bind(&CardLayer::touchbegin, this,
                                       std::placeholders::_1, std::placeholders::_2);
    listener->onTouchMoved = std::bind(&CardLayer::touchmove, this,
                                       std::placeholders::_1, std::placeholders::_2);
    listener->onTouchEnded = std::bind(&CardLayer::touchend, this,
                                       std::placeholders::_1, std::placeholders::_2);

    //init every card
    int i(0);
    auto cardUnitLayer = CSLoader::createNode(kCardUnitLayer);
    CardConfig *cc = base::Singleton<CardConfig>::get();
    AnimationConfig *ac = base::Singleton<AnimationConfig>::get();
    for(CardConfig::card_struct_map::iterator it = cc->cardMap.begin();
        it != cc->cardMap.end();
        it++)
    {
        Node *node = cardPosLayer->getChildByName(StringUtils::format("Node_1_0_%d",i));
        //card icon data
        CardStruct& card = it->second;
        CardUnit *sprite = CardUnit::create();
        
        //init properties
        AnimationConfig::role_struct_map::const_iterator roleIt = ac->roleStructMap.find(card.id);
        if(roleIt!=ac->roleStructMap.end())
        {
            const RoleStruct& role = roleIt->second;
            sprite->frame = CardFrame::create();
            sprite->addChild(sprite->frame);
            initRoleFrame(sprite->frame , cardUnitLayer, card, role);
        }
        
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(node->getPosition());
        sprite->originPos = sprite->getPosition();
        
        cardLayer->addChild(sprite);
        cardsel[i] = sprite;
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), sprite);
    
        i++;
    }
    
    std::string strpos = UserDefault::getInstance()->getStringForKey(kAppKeys[Ak_CARD_POS]);
    rapidjson::Document json;
    if(strpos.length()==0)
        goto end;
    json.Parse(strpos.c_str());
    assert(!json.HasParseError());
    for(rapidjson::Value::ConstMemberIterator itr = json.MemberBegin();
        itr != json.MemberEnd();
        ++itr)
    {
        int id = atoi(itr->name.GetString()) ;
        int pos = itr->value.GetInt();
        
        for(int j=0; j<(sizeof cardsel)/sizeof(CardUnit *); j++)
        {
            if( cardsel[j]->frame->id == id)
            {
                swapCards(cardsel[j], pos);
                break;
            }
        }
    }
    
end:
    return true;
}

void
CardLayer::initListener()
{}

void
CardLayer::updateCardsEvent(const rapidjson::Value& val)
{
    //init
    for(int j=0; j<(sizeof cardsel)/sizeof(CardUnit *); j++)
    {
        cardsel[j]->frame->setUpgrade();
    }
    
    for(rapidjson::Value::ConstMemberIterator it=val.MemberBegin();
        it!=val.MemberEnd();
        it++)
    {
        updateCard(it->value);
    }
}

CardFrame*
CardLayer::updateCard(const rapidjson::Value& val)
{
    int id = val["roleid"].GetInt();

    for(int j=0; j<(sizeof cardsel)/sizeof(CardUnit *); j++)
    {
        if( cardsel[j]->frame->id == id)
        {
            cardsel[j]->frame->upgrade = val["upgrade"].GetInt();
            cardsel[j]->frame->number = val["number"].GetInt();
            cardsel[j]->frame->level = val["level"].GetInt();
            cardsel[j]->frame->hp = val["hp"].GetInt();
            cardsel[j]->frame->attack = val["attack"].GetInt();
            cardsel[j]->frame->setUpgrade();
            return cardsel[j]->frame;
        }
    }
    return nullptr;
}

CardFrame*
CardLayer::getCardFrame(int id)
{
    for(int j=0; j<(sizeof cardsel)/sizeof(CardUnit *); j++)
    {
        if( cardsel[j]->frame->id == id)
        {
            return cardsel[j]->frame;
        }
    }
    return nullptr;
}

Vec2 gLastLocation(0,0);
float gDistance(0.0f);
int64_t gStartTime(0);
CardUnit* gCurTarget(nullptr);
bool gIsTouch(false);
bool gLongPress(false);
void
CardLayer::updatelongpress(float delta)
{
    if (gIsTouch&&gCurTarget&&gCurTarget->frame->level)
    {
        Vec2 locationInNode = gCurTarget->convertToNodeSpace(gLastLocation);
        Size s = gCurTarget->frame->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        //check is selected
        if (rect.containsPoint(locationInNode)
            || gLastLocation==Vec2::ZERO)
        {
            gLongPress=true;
        }
        
        //onLongPressed();
        //gCurTarget->cardGlow->setVisible(true);
        //gCurTarget->cardGlow->runAction(FadeIn::create(1)) ;
    }
}

bool
CardLayer::touchbegin(Touch* touch, Event* event)
{
    auto target = static_cast<CardUnit*>(event->getCurrentTarget());
    
    Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->frame->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    //check is selected
    if (rect.containsPoint(locationInNode))
    {
        target->setLocalZOrder(100);
        target->setOpacity(180);
        
        gLastLocation=touch->getLocation();
        //处理长按事件
        gCurTarget=target;
        gIsTouch=true;
        this->schedule(SEL_SCHEDULE(&::codechiev::CardLayer::updatelongpress), 0.3f);
        gLongPress=false;
        return true;
    }
    return false;
}
void
CardLayer::touchmove(Touch* touch, Event* event)
{
    gDistance += gLastLocation.distance(touch->getLocation());
    gLastLocation=touch->getLocation();
    if(gLongPress)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->setPosition(target->getPosition() + touch->getDelta());
    }
}

void
CardLayer::touchend(cocos2d::Touch *touch, cocos2d::Event *event)
{
    //处理长按事件
    if(gCurTarget && gDistance<10.0f)
    {
        CardDescLayer::show(gCurTarget->frame);
    }
    gCurTarget=nullptr;
    gIsTouch=false;
    gLongPress=false;
    gStartTime=0;
    gLastLocation=Vec2::ZERO;
    gDistance=0.0f;
    this->unschedule(SEL_SCHEDULE(&::codechiev::CardLayer::updatelongpress));
    
    auto target = static_cast<CardUnit*>(event->getCurrentTarget());
    target->setLocalZOrder(0);
    target->setOpacity(255);
    target->setPosition(target->originPos);
    
    Vec2 loc = touch->getLocation();
    for(int i=0;i<4;i++)
    {
        Vec2 locationInNode = cardsel[i]->convertToNodeSpace(loc);
        Size s = cardsel[i]->frame->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        //check is selected
        if (rect.containsPoint(locationInNode))
        {
            //check type
            if(cardsel[i]->frame->type!=target->frame->type)
            {
                gNotificationLayer->onError("只能选择相同属性的角色");
                break;
            }
            //check level
            if(target->frame->level)
            {
                swapCards(target, i);
                archiveCardPos();
                base::Singleton<BasisUtil>::get()->appDataSave();
            }
            
            break;
        }
    }
}

void
CardLayer::swapCards(CardUnit* target, int index)
{
    Vec2 pos = cardsel[index]->getPosition();
    cardsel[index]->setPosition(target->getPosition());
    cardsel[index]->originPos = target->getPosition();
    target->setPosition(pos);
    target->originPos = pos;
    
    CardUnit* temp=cardsel[index];
    for(int j=0; j<(sizeof cardsel)/sizeof(CardUnit *); j++)
    {
        if( cardsel[j]->frame->id == target->frame->id)
        {
            cardsel[j]=temp;
            cardposmap[temp->frame->id]=j;
            break;
        }
    }
    cardsel[index]=target;
    cardposmap[target->frame->id]=index;
}

void
CardLayer::onCreateCard()
{
    for(int j=0; j<(sizeof cardsel)/sizeof(CardUnit *); j++)
    {
        if(cardsel[j]->frame->level)
        {
            swapCards(cardsel[j], j%4);
        }
    }
    archiveCardPos();
}

void
CardLayer::archiveCardPos()
{
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    for(card_pos_map::const_iterator it=cardposmap.begin();
        it!=cardposmap.end();
        it++)
    {
        writer.Key(StringUtils::format("%d", it->first).c_str() );
        writer.Uint(it->second);
        //log("%d->%d",it->first,it->second);
    }
    writer.EndObject();
    UserDefault::getInstance()->setStringForKey(kAppKeys[Ak_CARD_POS], sb.GetString()) ;
    base::Singleton<BasisUtil>::get()->appDataSave();
}

void
CardConfig::init()
{
    auto fu = FileUtils::getInstance();
    std::string fullname=fu->fullPathForFilename(kPlistCard);
    ValueMap valMap = fu->getValueMapFromFile(fullname);
    //init card from frames
    ValueMap& frames = valMap["frames"].asValueMap();
    std::set<std::string> names;//names in order
    for(ValueMap::iterator framesIt = frames.begin();
        framesIt != frames.end();
        framesIt++)
    {
        std::string name = framesIt->first;
        names.insert(name);
    }
    
    int id(1);
    for(std::set<std::string>::iterator namesIt = names.begin();
        namesIt != names.end();
        namesIt++)
    {
        //log( (*namesIt).c_str());
        CardStruct& card = cardMap[id];
        card.id = id;
        card.src = *namesIt;
        id++;
    }
}
//
//  MessageLayer.cpp
//  empire
//
//  Created by metasoft on 16/5/9.
//
//

#include "MessageLayer.hpp"
#include "RedeemLayer.hpp"
#include "NavigateLayer.hpp"
#include "User.h"
#include "../codechiev/BasisUtil.h"
#include "../model/Effect.hpp"
#include "../codechiev/control/ChatController.hpp"
#include "../codechiev/control/LoginController.h"
#include "../resource.h"
#include "../layer/GameLayer.h"
#include "../layer/MenuLayer.h"
#include "../layer/NotificationLayer.h"
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace codechiev;

MessageLayer::MessageLayer(){}

//touch begin
Vec2 touchBegin(0,0);

bool
MessageLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    auto msgLayer = CSLoader::createNode(kMsgLayer);
    msgLayer->setPosition(gCenter);
    this->addChild(msgLayer);
    
    ui::Layout *layout = static_cast<ui::Layout*>(msgLayer->getChildByName("Panel_Msg"));
    listView = ui::ListView::create();
    listView->setContentSize(layout->getContentSize());
    listView->setPosition(layout->getPosition()  );
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    //listView->setItemsMargin(30);
    listView->setScrollBarEnabled(false);
    listView->setSwallowTouches(true);//not affect swipe guesture
    msgLayer->addChild(listView);
    //======registEvent======
    codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.chat", std::bind([](rapidjson::Document& doc, MessageLayer* layer){
        const rapidjson::Value& data = doc["data"];
        auto layout = layer->chatListAdd(data);
        layout->setScale(.3f);
        layout->runAction(Sequence::create(ScaleTo::create(.1f, 1.0f), NULL));
    }, std::placeholders::_1, this));
    codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.chat.invite", std::bind([](rapidjson::Document& doc, MessageLayer* layer){
        const rapidjson::Value& data = doc["data"];
        auto layout = layer->chatListAdd(data);
        layout->setScale(.3f);
        layout->runAction(Sequence::create(ScaleTo::create(.1f, 1.0f), NULL));
    }, std::placeholders::_1, this));
    codechiev::base::Singleton<CmdCallback>::get()->registEvent("event.chat.fangpiao", std::bind([](rapidjson::Document& doc, MessageLayer* layer){
        const rapidjson::Value& data = doc["data"];
        auto layout = layer->chatListAdd(data);
        layout->setScale(.3f);
        layout->runAction(Sequence::create(ScaleTo::create(.1f, 1.0f), NULL));
    }, std::placeholders::_1, this));
    
    
    msgBtn = static_cast<ui::Button*>(msgLayer->getChildByName("Btn_Input"));
    msgBtn->addClickEventListener(std::bind([](Button* btn){
        showKeyboard();
        btn->setEnabled(false);
    }, msgBtn));
    
    auto redeemBtn = static_cast<ui::Button*>(msgLayer->getChildByName("BTN_Redeem"));
    redeemBtn->addClickEventListener(std::bind([=](Ref* ref){
        auto redeemLayer = RedeemLayer::create();
        redeemLayer->setVisible(false);
        gMenuLayer->addChild(redeemLayer);
        redeemLayer->doModal();
    }, std::placeholders::_1));
    return true;
}

void
MessageLayer::chatListOk(rapidjson::Document& doc)
{
    const rapidjson::Value& data = doc["data"];
    const rapidjson::Value& list = data["list"];
    assert(list.IsArray());
    for (rapidjson::SizeType i = 0; i < list.Size(); i++)
    {
        const rapidjson::Value& data = list[i];
        if(data.IsNull())
            continue;
        chatListAdd(data);
    }
}

#define MARGIN 90
#define MIN_HEIGHT 98
#define FONT_SIZE 36
#define LINE_HEIGHT 43
#define MSG_BOTTOM_MARGIN 22
#define MSG_TOP_MARGIN 12
Layout*
MessageLayer::chatListAdd(const rapidjson::Value & data)
{
    auto layout = Layout::create();
    layout->setAnchorPoint(Vec2::ZERO);
    layout->setPosition(Vec2::ZERO);
    
    const rapidjson::Value& type = data["type"];
    if(type.GetInt()==1)
    {
        addChat(layout, data);
    }
    else if(type.GetInt()==2)
    {
        addInvite(layout, data);
    }
    else if(type.GetInt()==4)
    {
        addRedeem(layout, data);
    }

    
    if(listView->getItems().size()>20)
    {
        listView->removeItem(0);
    }
    
    listView->jumpToBottom();
    
    return layout;
}

void
MessageLayer::addChat(Layout *layout, const rapidjson::Value & data)
{
    const rapidjson::Value& msg = data["msg"];
    const rapidjson::Value& name = data["name"];
    bool self(false);
    std::string namestr = name.IsNull()?"":name.GetString();
    if(namestr.length()&&namestr.compare(base::Singleton<User>::get()->name)==0)
    {
        self=true;
    }
    
    float lvwidth = listView->getContentSize().width;
    float innerwidth = lvwidth-MARGIN;
    
    auto richText = RichText::create();
    richText->pushBackElement(RichElementText::create(0, Color3B( 0x1b, 0x94, 0xd3), 0xffffff, namestr, "", 20));
    richText->pushBackElement(RichElementNewLine::create(0, Color3B(0,0,0), 0x0));
    richText->pushBackElement(RichElementText::create(1, Color3B(0x01,0x01,0x01), 255, msg.GetString(), "", FONT_SIZE));
    richText->formatText();
    richText->ignoreContentAdaptWithSize(false);
    const Size &size = richText->getContentSize();
    auto background = Scale9Sprite::create(kImgMsgBg, Rect(0, 0, 250, 98), Rect(18,55, 196, 22));
    richText->setContentSize(Size(size.width<innerwidth?size.width:innerwidth,
                        size.height + (std::floor(size.width / innerwidth)) * LINE_HEIGHT));
    float bgHeight = size.height<MIN_HEIGHT?MIN_HEIGHT:size.height+MSG_BOTTOM_MARGIN+MSG_TOP_MARGIN;log("bg height: %g->%g", size.height, bgHeight);
    background->setPreferredSize(Size(size.width+MARGIN, bgHeight));
    if(self)
    {
        background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        background->setPosition(Vec2(lvwidth, 0));
        richText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        richText->setPosition(Vec2(lvwidth-MARGIN*.5f, MSG_BOTTOM_MARGIN));
    }
    else
    {
        background->setFlippedX(true);
        background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        background->setPosition(Vec2(size.width+MARGIN, 0));
        richText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        richText->setPosition(Vec2(MARGIN*.5f, MSG_BOTTOM_MARGIN));
    }

    layout->addChild(background,-1);
    layout->setContentSize(Size(lvwidth, background->getContentSize().height));
    layout->addChild(richText);
    listView->pushBackCustomItem(layout);

}

void
MessageLayer::addInvite(Layout *layout, const rapidjson::Value & data)
{
    const rapidjson::Value& msg = data["msg"];
    const rapidjson::Value& name = data["name"];
    std::string namestr = name.IsNull()?"":name.GetString();
    bool self(false);
    if(namestr.length()&&namestr.compare(base::Singleton<User>::get()->name)==0)
    {
        self=true;
    }

    float lvwidth = listView->getContentSize().width;
    
    auto csd = CSLoader::createNode(kMsgNode);
    auto sprite = csd->getChildByName("Sprite_Invite");
    auto btn = sprite->getChildByName("Btn");
    auto richText = RichText::create();
    richText->pushBackElement(RichElementText::create(0, Color3B( 0x1b, 0x94, 0xd3), 0xffffff, namestr, "", 20));
    richText->pushBackElement(RichElementNewLine::create(0, Color3B(0,0,0), 0x0));
    auto elem = RichElementText::create(1, Color3B(0x01,0x01,0x01), 255, msg.GetString(), "", 18);
    richText->pushBackElement(elem);
    richText->formatText();
    richText->ignoreContentAdaptWithSize(false);
    auto background = Scale9Sprite::create(kImgIviteMsg, Rect(0, 0, 559, 148), Rect(20,12, 520, 124));
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    background->setPreferredSize(Size(lvwidth, 148));
    const Size &size = background->getPreferredSize();
    richText->setContentSize(size);
    richText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    richText->setPosition(Vec2(MARGIN*.3f, size.height-MARGIN*.3f));
    
    auto btnAccept = BufuBtn::create();
    btnAccept->init(kImgBufu, kImgBufuPress);
    if(self)
        btnAccept->setEnabled(false);
    btnAccept->setPosition(btn->getPosition());
    btnAccept->addClickEventListener(std::bind(&BufuBtn::onPress,
                                               btnAccept, std::placeholders::_1));
    layout->addChild(btnAccept);
    layout->addChild(background,-1);
    layout->setContentSize( size);
    layout->addChild(richText);
    listView->pushBackCustomItem(layout);
}
void
MessageLayer::addRedeem(Layout *layout, const rapidjson::Value & data)
{
    const rapidjson::Value& msg = data["msg"];
    const rapidjson::Value& name = data["name"];
    const rapidjson::Value& num = data["num"];
    const rapidjson::Value& key = data["key"];
    std::string namestr = name.IsNull()?"":name.GetString();
    bool self(false);
    if(namestr.length()&&namestr.compare(base::Singleton<User>::get()->name)==0)
    {
        self=true;
    }
    
    float lvwidth = listView->getContentSize().width;
    
    auto csd = CSLoader::createNode(kMsgNode);
    auto sprite = csd->getChildByName("Sprite_Redeem");
    auto btn = sprite->getChildByName("Btn");
    auto richText = RichText::create();
    richText->pushBackElement(RichElementNewLine::create(0, Color3B(0,0,0), 0x0));
    auto elem = RichElementText::create(1, Color3B(0x01,0x01,0x01), 255, msg.GetString(), "", 18);
    richText->pushBackElement(elem);
    richText->formatText();
    richText->ignoreContentAdaptWithSize(false);
    auto background = Scale9Sprite::create(kImgRedeemMsg, Rect(0, 0, 584, 168), Rect(20,12, 520, 124));
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    background->setPreferredSize(Size(lvwidth, 148));
    const Size &size = background->getPreferredSize();
    richText->setContentSize(Size(250,100));
    richText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    richText->setPosition(Vec2(110, 100));
    
    auto redeemBtn = RedeemBtn::create();
    redeemBtn->init(kImgToguan, kImgToguanPress);
    if(num.GetInt()==0 || self)
        redeemBtn->setEnabled(false);
    redeemBtn->setPosition(btn->getPosition());
    redeemBtn->key=key.GetString();
    redeemBtn->addClickEventListener(std::bind(&RedeemBtn::onPress,
                                               redeemBtn, std::placeholders::_1));
    layout->addChild(redeemBtn);
    layout->addChild(background,-1);
    layout->setContentSize( size);
    layout->addChild(richText);
    listView->pushBackCustomItem(layout);
}
#undef MARGIN
#undef MIN_HEIGHT
void
BufuBtn::onPress(cocos2d::Ref *)
{
    this->setEnabled(false);
}
void
RedeemBtn::onPress(cocos2d::Ref *)
{
    this->setEnabled(false);
    control::LoginController::redeemRecv(this->key, CmdCallbackBegin
                                         if(ok)
                                         {
                                             gNotificationLayer->onError("托管成功");
                                         }
                                         CmdCallbackEnd);
}



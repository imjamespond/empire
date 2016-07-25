//
//  MessageLayer.hpp
//  empire
//
//  Created by metasoft on 16/5/9.
//
//

#ifndef MessageLayer_hpp
#define MessageLayer_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <external/json/document.h>

static const char kMsgNode[] = "ui/MsgNode.csb";

namespace codechiev
{
    class BufuBtn;
    class RedeemBtn;
    class MessageLayer : public cocos2d::Layer
    {
    public:
        MessageLayer();
        
        CREATE_FUNC(MessageLayer);
        
        virtual bool init();
        void chatListOk(rapidjson::Document&);
        cocos2d::ui::Layout* chatListAdd(const rapidjson::Value&);
        void addChat(cocos2d::ui::Layout *, const rapidjson::Value &);
        void addInvite(cocos2d::ui::Layout *, const rapidjson::Value &);
        void addRedeem(cocos2d::ui::Layout *, const rapidjson::Value &);
        
        cocos2d::ui::ListView* listView;
        cocos2d::ui::Button* msgBtn;
    private:
    };
    
    class BufuBtn : public cocos2d::ui::Button
    {
    public:
        CREATE_FUNC(BufuBtn);
        void onPress(cocos2d::Ref*);
        
        std::string key;
    };
    class RedeemBtn : public cocos2d::ui::Button
    {
    public:
        CREATE_FUNC(RedeemBtn);
        void onPress(cocos2d::Ref*);
        
        std::string key;
    };
}

#endif /* MessageLayer_hpp */

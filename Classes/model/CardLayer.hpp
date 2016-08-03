//
//  CardLayer.hpp
//  empire
//
//  Created by metasoft on 16/5/9.
//
//

#ifndef CardLayer_hpp
#define CardLayer_hpp
#include "ActionSprite.h"
#include "ModalLayer.hpp"
#include "UpgradeOkLayer.hpp"
#include <external/json/document.h>
#include <cocos2d.h>
#include <ui/CocosGUI.h>

static const char kCardDescLayer[] = "ui/CardDescLayer.csb";
static const char kCardDescPosLayer[] = "ui/CardDescPosLayer.csb";

namespace codechiev
{
    extern std::string getRoleName(int roleid);
    extern const struct RoleStruct* getRole(int roleid);
    
    struct CardStruct
    {
        CardStruct():id(0){}
        int id;
        std::string src;
    };
    extern const CardStruct* getCard(int roleid);
    
    class CardConfig
    {
    public:
        CardConfig(){}
        void init();
        
        typedef std::map<int, struct CardStruct> card_struct_map;
        card_struct_map cardMap;
    };
    
    class CardFrame : public cocos2d::Sprite
    {
    public:
        CardFrame();
        CREATE_FUNC(CardFrame);
        
        virtual bool init();
        
        void setUpgrade();
        void change();
        void changeLarge();
        void operator=(const CardFrame*);
        inline cocos2d::EventListener* getListener(){assert(listener);return listener;};
        
        int id;
        int type;//1 indicate selected
        int number;
        int upgrade;
        int level;
        int attack;
        int hp;
        float expbarWidth;
        cocos2d::ui::Scale9Sprite *expbar;
        cocos2d::ClippingNode *clip;
        cocos2d::Sprite *cardFrame;
        cocos2d::Sprite *cardGlow;
        cocos2d::Sprite *spriteFrame;
        cocos2d::ui::Text *upgradeText;
        cocos2d::ui::Text *levelText;
        cocos2d::EventListener* listener;
    };
    
    class CardUnit : public cocos2d::Sprite
    {
    public:
        CardUnit();
        
        CREATE_FUNC(CardUnit);
        
        virtual bool init();

        cocos2d::Vec2 originPos;
        CardFrame* frame;
    };
    
    class UpgradeOkLayer;
    class CardDescLayer : public ModalLayer
    {
    public:
        CREATE_FUNC(CardDescLayer);
        
        virtual bool init();
        
        static void show( CardFrame*);
        void onUpgrade(cocos2d::Ref*);
        
        UpgradeOkLayer *upgradeOkLayer;
    private:
        CardFrame* frame;
    };
    
    class CardLayer : public cocos2d::Layer
    {
    public:
        CardLayer();
        
        CREATE_FUNC(CardLayer);
        
        virtual bool init();
        void updateCardsEvent(const rapidjson::Value&);
        CardFrame* getCardFrame(int);
        CardFrame* updateCard(const rapidjson::Value&);
        void initListener();
        void updatelongpress(float );
        bool touchbegin(cocos2d::Touch* touch, cocos2d::Event* event);
        void touchmove(cocos2d::Touch* touch, cocos2d::Event* event);
        void touchend(cocos2d::Touch* touch, cocos2d::Event* event);
        void swapCards(CardUnit*, int);
        void onCreateCard();
        void archiveCardPos();
        
        CardUnit *cardsel[16];
        typedef std::map<int, int> card_pos_map;
        card_pos_map cardposmap;
    private:
    };

}

#endif /* CardLayer_hpp */

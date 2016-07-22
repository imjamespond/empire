//
//  UpgradeOkLayer.hpp
//  empire
//
//  Created by metasoft on 16/6/30.
//
//

#ifndef UpgradeOkLayer_hpp
#define UpgradeOkLayer_hpp

#include "ModalLayer.hpp"
#include <json/document.h>
#include <cocos2d.h>
#include <ui/CocosGUI.h>

static const char kUpgradeOkLayer[] = "ui/UpgradeOkLayer.csb";
static const char kUpgradeOkPosLayer[] = "ui/UpgradeOkPosLayer.csb";

namespace codechiev
{
    class CardFrame;
    
    class UpgradeOkLayer : public ModalLayer
    {
    public:
        UpgradeOkLayer();
        
        CREATE_FUNC(UpgradeOkLayer);
        
        virtual bool init();
        static void show(cocos2d::Node*, const rapidjson::Value& );
        void hide();
        void updateFrame(const CardFrame*);
        
        CardFrame *frame;
        cocos2d::ui::Text *attack;
        cocos2d::ui::Text *hp;
        cocos2d::ui::Text *name;
    };
}

#endif /* UpgradeOkLayer_hpp */

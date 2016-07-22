//
//  RecieveOkLayer.hpp
//  empire
//
//  Created by metasoft on 16/7/1.
//
//

#ifndef RecieveOkLayer_hpp
#define RecieveOkLayer_hpp


#include "ModalLayer.hpp"
#include <json/document.h>
#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <deque>

static const char kRecieveOkLayer[] = "ui/RecieveOkLayer.csb";
static const char kRecieveOkPosLayer[] = "ui/RecieveOkPosLayer.csb";

namespace codechiev
{
    class CardFrame;
    
    class RecieveOkLayer : public ModalLayer
    {
    public:
        RecieveOkLayer();
        
        CREATE_FUNC(RecieveOkLayer);
        
        virtual bool init();
        static void show(cocos2d::Node*, const rapidjson::Value& );
        void hide();
        
        typedef std::deque<CardFrame *> frame_deq;
        frame_deq frames;
        CardFrame *curFrame;
        cocos2d::ui::Text *name;
    };
}

#endif /* RecieveOkLayer_hpp */

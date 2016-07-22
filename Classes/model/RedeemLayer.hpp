//
//  RedeemLayer.hpp
//  empire
//
//  Created by metasoft on 16/6/28.
//
//

#ifndef RedeemLayer_hpp
#define RedeemLayer_hpp

#include "ModalLayer.hpp"
#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <unordered_map>

static const char kRedeemLayer[] = "ui/RedeemLayer.csb";
static const char kRedeemPosLayer[] = "ui/RedeemPosLayer.csb";

namespace codechiev
{
    class ConfirmLayer;
    class CardFrame;
    class RedeemLayer : public ModalLayer
    {
    public:
        RedeemLayer();
        
        CREATE_FUNC(RedeemLayer);
        
        virtual bool init();
        virtual void doModal();
        virtual void undoModal();
        bool touchbegin(cocos2d::Touch* , cocos2d::Event* );
        void updateFrame(CardFrame*);
        
        ConfirmLayer* confirm;
        typedef std::unordered_map<int, CardFrame*> frame_map;
        frame_map frames;
    };
}

#endif /* RedeemLayer_hpp */

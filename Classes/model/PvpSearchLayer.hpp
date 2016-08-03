//
//  PvpSearchLayer.hpp
//  empire
//
//  Created by metasoft on 16/6/29.
//
//

#ifndef PvpSearchLayer_hpp
#define PvpSearchLayer_hpp

#include "ModalLayer.hpp"
#include "Effect.hpp"
#include <cocos2d.h>
#include <ui/CocosGUI.h>

static const char kPvpSearchLayer[] = "ui/PvpSearchLayer.csb";

namespace codechiev
{
    class PvpSearchLayer : public ModalLayer
    {
    public:
        PvpSearchLayer();
        
        CREATE_FUNC(PvpSearchLayer);
        
        virtual bool init();
        virtual void undoModal();
        virtual void doModal();
        void countdownUpdate(float dt);
        
        cocos2d::Node* rotateNode;
        //cocos2d::Node* rotateGlass;
        
        float left;
    };
}

#endif /* PvpSearchLayer_hpp */

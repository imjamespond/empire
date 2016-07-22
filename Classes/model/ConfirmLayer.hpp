//
//  ConfirmLayer.hpp
//  empire
//
//  Created by metasoft on 16/7/4.
//
//

#ifndef ConfirmLayer_hpp
#define ConfirmLayer_hpp

#include "ModalLayer.hpp"
#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

static const char kConfirmLayer[] = "ui/ConfirmLayer.csb";

namespace codechiev {
    
    class ConfirmLayer : public ModalLayer
    {
    public:
        typedef boost::function<void()> confirm_callback;
        
        CREATE_FUNC(ConfirmLayer);
        
        virtual bool init();
        virtual void undoModal();
        
        void pauseEvent(cocos2d::Node*);
        void resumeEvent(cocos2d::Node*);
        void confirm(cocos2d::Ref* ref);
        
        confirm_callback cb;
        cocos2d::ui::Text *text;
        cocos2d::Node* target;
    };
}

#endif /* ConfirmLayer_hpp */

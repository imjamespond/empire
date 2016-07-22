//
//  ModalLayer.hpp
//  empire
//
//  Created by metasoft on 16/6/17.
//
//

#ifndef ModalLayer_hpp
#define ModalLayer_hpp

#include <cocos2d.h>

namespace codechiev
{
    class ModalLayer : public cocos2d::Layer
    {
    public:
        CREATE_FUNC(ModalLayer);
        virtual bool init();
        virtual void onClose(cocos2d::Ref*);
        void initModal(cocos2d::Layer*);
        virtual void doModal();
        virtual void undoModal();
        
        cocos2d::Layer* modalLayer;
        
        typedef std::vector<ModalLayer*> modal_vec;
        static modal_vec modals;
    };
}

#endif /* ModalLayer_hpp */

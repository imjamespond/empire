//
//  ExchangeLayer.hpp
//  empire
//
//  Created by metasoft on 16/6/30.
//
//

#ifndef ExchangeLayer_hpp
#define ExchangeLayer_hpp

#include "ModalLayer.hpp"
#include <cocos2d.h>
#include <ui/CocosGUI.h>

static const char kExchangeLayer[] = "ui/ExchangeLayer.csb";

namespace codechiev
{
    class ExchangeLayer : public ModalLayer
    {
    public:
        ExchangeLayer();
        
        CREATE_FUNC(ExchangeLayer);
        
        virtual bool init();
        
        void exchange(int );
    };
}

#endif /* ExchangeLayer_hpp */

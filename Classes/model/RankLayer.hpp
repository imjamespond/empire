//
//  RankLayer.hpp
//  empire
//
//  Created by metasoft on 16/6/17.
//
//

#ifndef RankLayer_hpp
#define RankLayer_hpp

#include "ModalLayer.hpp"
#include <cocos2d.h>
#include <ui/CocosGUI.h>

namespace codechiev
{
    static const char kImgRankNum[] = "img/ranknum%d.png";
    
    class RankLayer : public ModalLayer
    {
    public:
        RankLayer();
        
        CREATE_FUNC(RankLayer);
        
        virtual bool init();
        virtual void onClose(cocos2d::Ref*);
        
        void rankScore();
        void rankRedeem();
        void onRadioChange(cocos2d::ui::RadioButton* , int , cocos2d::ui::RadioButtonGroup::EventType );
        
    };
}

#endif /* RankLayer_hpp */

//
//  CycleShowPageView.hpp
//  empire
//
//  Created by metasoft on 16/6/14.
//
//

#ifndef CycleShowPageView_hpp
#define CycleShowPageView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

namespace codechiev
{
    class CycleShowPageView : public cocos2d::ui::PageView
    {
    public:
        CREATE_FUNC(CycleShowPageView);
        
        virtual bool init();
        virtual void moveInnerContainer(const cocos2d::Vec2& , bool );
    };
}

#endif /* CycleShowPageView_hpp */

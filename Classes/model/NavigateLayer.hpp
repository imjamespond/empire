//
//  MainUI.hpp
//  empire
//
//  Created by metasoft on 16/4/21.
//
//

#ifndef MainUI_hpp
#define MainUI_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class MenuLayer;

namespace codechiev
{
    class NavPageView;
    
    class NavigateLayer : public cocos2d::Layer
    {
    public:
        NavigateLayer();
        
        CREATE_FUNC(NavigateLayer);
        
        virtual bool init();

        void initNav(MenuLayer*);
        void onRadioChange(cocos2d::ui::RadioButton* , int , cocos2d::ui::RadioButtonGroup::EventType );
        
        cocos2d::Sprite *cursor;
        cocos2d::ui::RadioButtonGroup *radioGroup;
        NavPageView *pageView;
        
    private:
    };
    
    class NavPageView: public cocos2d::ui::PageView
    {
    public:
        NavPageView();
        
        CREATE_FUNC(NavPageView);
        
        virtual void moveInnerContainer(const cocos2d::Vec2& deltaMove, bool canStartBounceBack);
        virtual void handleReleaseLogic(cocos2d::Touch *touch);
        
        NavigateLayer* mainui;
    };
    
}

#endif /* MainUI_hpp */

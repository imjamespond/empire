//
//  HallLayer.hpp
//  empire
//
//  Created by metasoft on 16/5/11.
//
//

#ifndef HallLayer_hpp
#define HallLayer_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "ModalLayer.hpp"
#include "MessageLayer.hpp"


namespace codechiev
{
    class IntroLayer;
    
    class HallLayer : public cocos2d::Layer
    {
    public:
        HallLayer();
        
        CREATE_FUNC(HallLayer);
        
        virtual bool init();
        
        void gameAlloc(cocos2d::Ref* );
        void updateScore(int score);
        void updateLevel(int level);
        void gamePveAlloc();
        void stopCountDown();
        void onRanklist(cocos2d::Ref*);
        void onRecieve(cocos2d::Ref*);
        void onIntro(cocos2d::Ref*);
        void onExchange(cocos2d::Ref*);
        
        cocos2d::ui::Text *txName;
        cocos2d::ui::Text *txScore;
        cocos2d::ui::Text *txLevel;
        cocos2d::ui::Button *btnBegin;
        cocos2d::ui::Button *btnConfig;
        cocos2d::ui::PageView *pageViewMap;
        
    private:
    };

    class MapImgView:public cocos2d::ui::ImageView
    {
    public:
        CREATE_FUNC(MapImgView);
        
        int mapid;
    };
    
    class IntroLayer : public ModalLayer
    {
    public:
        CREATE_FUNC(IntroLayer);
        
        virtual bool init();
    };
}

#endif /* HallLayer_hpp */

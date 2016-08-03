//
//  LoadingLayer.hpp
//  empire
//
//  Created by metasoft on 16/5/6.
//
//

#ifndef LoadingLayer_hpp
#define LoadingLayer_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LoadingLayer;
extern LoadingLayer *gLoadingLayer;

class LoadingLayer : public cocos2d::Layer
{
public:
    LoadingLayer();
    
    CREATE_FUNC(LoadingLayer);
    
    virtual bool init();
    virtual void update(float);
    
    cocos2d::Node* loadingNode;
    cocos2d::ui::LoadingBar *loadingBar;
    cocos2d::ui::TextBMFont *percentBMF;
    
    float percentage;
private:
    void onLoading();
    void onOpen();
};

#endif /* LoadingLayer_hpp */

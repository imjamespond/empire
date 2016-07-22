//
//  GameEndLayer.hpp
//  empire
//
//  Created by metasoft on 16/5/5.
//
//

#ifndef GameEndLayer_hpp
#define GameEndLayer_hpp

#include <cocos2d.h>

static const char kGameEndLayer[] = "ui/BattleEnd.csb";

class GameEndLayer : public cocos2d::Layer
{
public:
    GameEndLayer();
    
    CREATE_FUNC(GameEndLayer);
    
    virtual bool init();
    
    static void show(int64_t, int);
    bool hide(cocos2d::Touch* , cocos2d::Event* );
    
    cocos2d::Node* node;
private:
};

#endif /* GameEndLayer_hpp */

//
//  ScreenShake.hpp
//  empire
//
//  Created by metasoft on 16/4/19.
//
//

#ifndef ScreenShake_hpp
#define ScreenShake_hpp

#include "cocos2d.h"

class ScreenShake : public cocos2d::ActionInterval
{
public:
    ScreenShake();
    
    // Create the action with a time and a strength (same in x and y)
    static ScreenShake *create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static ScreenShake *createWithStrength(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
protected:
    
    virtual void startWithTarget(cocos2d::Node *);
    virtual void update(float );
    virtual void stop();
    
    cocos2d::Vec2 m_StartPosition;
    cocos2d::Node* m_pTarget;
    
    // Strength of the action
    float m_strength_x, m_strength_y;
};


#endif /* ScreenShake_hpp */

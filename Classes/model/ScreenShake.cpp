//
//  ScreenShake.cpp
//  empire
//
//  Created by metasoft on 16/4/19.
//
//

#include "ScreenShake.hpp"

using namespace cocos2d;

// not really useful, but I like clean default constructors
ScreenShake::ScreenShake() : m_strength_x(0), m_strength_y(0),
m_pTarget(nullptr)
{
}

ScreenShake *ScreenShake::create( float d, float strength )
{
    // call other construction method with twice the same strength
    return createWithStrength( d, strength, strength );
}

ScreenShake *ScreenShake::createWithStrength(float duration, float strength_x, float strength_y)
{
    ScreenShake *pRet = new ScreenShake();
    
    if (pRet && pRet->initWithDuration(duration, strength_x, strength_y))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    
    return pRet;
}

bool ScreenShake::initWithDuration(float duration, float strength_x, float strength_y)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_strength_x = strength_x;
        m_strength_y = strength_y;
        
        return true;
    }
    
    return false;
}

// Helper function. I included it here so that you can compile the whole file
// it returns a random value between min and max included
static float fgRangeRand( float min, float max )
{
    float rnd = ((float)rand() / (float)RAND_MAX);
    return rnd * (max - min) + min;
}

void ScreenShake::update(float dt)
{
    float randx = fgRangeRand( -m_strength_x, m_strength_x ) * dt;
    float randy = fgRangeRand( -m_strength_y, m_strength_y ) * dt;
    
    // move the target to a shaked position
    m_pTarget->setPosition( m_StartPosition + Vec2( randx, randy));
}

void ScreenShake::startWithTarget(Node *pTarget)
{
    ActionInterval::startWithTarget( pTarget );
    
    // save the initial position
    m_StartPosition=pTarget->getPosition();
    m_pTarget = pTarget;
}

void ScreenShake::stop(void)
{
    // Action is done, reset clip position
    this->getTarget()->setPosition( m_StartPosition);
    
    ActionInterval::stop();
}
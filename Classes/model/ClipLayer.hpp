//
//  ClipLayer.hpp
//  empire
//
//  Created by metasoft on 16/4/19.
//
//

#ifndef Clipper_hpp
#define Clipper_hpp

#include "cocos2d.h"

class ClipHole : public cocos2d::Layer
{
public:
    ~ClipHole();
    
    CREATE_FUNC(ClipHole);
    
    virtual bool init();
    
    void addHoleAtPoint(cocos2d::Vec2 );
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
private: 
    cocos2d::ClippingNode* clipingNode;
    cocos2d::Node* _holeEdges;
    cocos2d::Node* _holeStencils;
};

class ClipScroll : public cocos2d::Layer
{
public:
    CREATE_FUNC(ClipScroll);
    
    virtual bool init();
    
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
private:
    bool _scrolling;
    cocos2d::Vec2 _lastPoint;
    
    cocos2d::ClippingNode *clip;
    cocos2d::Sprite *content;
};

#endif /* Clipper_hpp */

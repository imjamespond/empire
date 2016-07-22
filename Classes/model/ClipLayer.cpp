//
//  ClipLayer.cpp
//  empire
//
//  Created by metasoft on 16/4/19.
//
//

#include "ClipLayer.hpp"
#include "../codechiev/BasisUtil.h"
#include "../resource.h"

using namespace cocos2d;

ClipHole::~ClipHole(){}

bool
ClipHole::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    //clipingNode
    clipingNode = ClippingNode::create();
    clipingNode->setAnchorPoint( Vec2(0.5, 0.5) );
    clipingNode->setPosition(gCenter);
    //stencil 剪切模
    auto target = Sprite::create(kBlocks);
    target->setAnchorPoint(Vec2::ZERO);
    target->setScale(3);
    clipingNode->setStencil( target );
    AffineTransform tranform = AffineTransform::IDENTITY;
    tranform = AffineTransformScale(tranform, target->getScale(), target->getScale());
    //clipingNode->runAction(RepeatForever::create(RotateBy::create(1, 45)));
    clipingNode->setContentSize( SizeApplyAffineTransform(target->getContentSize(), tranform));
    //clipingNode 弹孔图片剪辑
    auto holesClippingNode = ClippingNode::create();
    holesClippingNode->setInverted(true);//if not,it'll shows edge inside the bullethole
    holesClippingNode->setAlphaThreshold( 0.9f );
    holesClippingNode->addChild( target );//hole edge background
    _holeEdges = Node::create();
    holesClippingNode->addChild(_holeEdges);
    //stencils 弹孔剪切模
    _holeStencils = Node::create();
    holesClippingNode->setStencil(_holeStencils);
    clipingNode->addChild(holesClippingNode);
    this->addChild(clipingNode);
    
    auto listener = EventListenerTouchOneByOne::create();
    //auto listener = EventListenerTouchAllAtOnce::create();//multiple touches
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ClipHole::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void
ClipHole::addHoleAtPoint(Vec2 point)
{
    float scale = CCRANDOM_0_1() * 0.2 + 0.9;
    float rotation = CCRANDOM_0_1() * 360;
    
    auto holeEdge = Sprite::create(kHoleEdge);
    holeEdge->setPosition( point );
    holeEdge->setRotation( rotation );
    holeEdge->setScale( scale );
    _holeEdges->addChild(holeEdge);
    
    auto holeStencil = Sprite::create(kHoleStencil);
    holeStencil->setPosition( point );
    holeStencil->setRotation( rotation );
    holeStencil->setScale( scale );
    _holeStencils->addChild(holeStencil);
    
    clipingNode->runAction(Sequence::createWithTwoActions(ScaleBy::create(0.05f, 0.95f),ScaleTo::create(0.125f, 1)));
}

bool
ClipHole::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 point = clipingNode->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
    auto rect = Rect(0, 0, clipingNode->getContentSize().width, clipingNode->getContentSize().height);
    if (!rect.containsPoint(point)) return false;
    this->addHoleAtPoint(point);
    return true;
}

bool
ClipScroll::init()
{
    if(!Layer::init())
    {
        return false;
    }
    //clip node
    clip = ClippingNode::create();
    //clip->setTag( kTagClipperNode );
    clip->setContentSize(  Size(200, 200) );
    clip->setAnchorPoint(  Vec2(0.5, 0.5) );
    clip->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    clip->runAction(RepeatForever::create(RotateBy::create(1, 45)));
    this->addChild(clip);
    //stencil
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(clip->getContentSize().width, 0);
    rectangle[2] = Vec2(clip->getContentSize().width, clip->getContentSize().height);
    rectangle[3] = Vec2(0, clip->getContentSize().height);
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clip->setStencil(stencil);
    //content
    content = Sprite::create(kBlocks);
    content->setAnchorPoint(  Vec2(0.5, 0.5) );
    content->setPosition(clip->getContentSize().width / 2, clip->getContentSize().height / 2);
    clip->addChild(content);
    
    _scrolling = false;
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(ClipScroll::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(ClipScroll::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(ClipScroll::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void
ClipScroll::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
    Touch *touch = touches[0];
    Vec2 point = clip->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
    auto rect = Rect(0, 0, clip->getContentSize().width, clip->getContentSize().height);
    _scrolling = rect.containsPoint(point);
    _lastPoint = point;
}

void
ClipScroll::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if (!_scrolling) return;
    Touch *touch = touches[0];
    auto point = clip->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
    Vec2 diff = point - _lastPoint;
    content->setPosition(content->getPosition() + diff);
    _lastPoint = point;
}

void
ClipScroll::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    if (!_scrolling) return;
    _scrolling = false;
}




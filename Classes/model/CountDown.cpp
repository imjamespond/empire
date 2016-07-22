//
//  CountDownTimer.cpp
//  empire
//
//  Created by metasoft on 16/4/20.
//
//

#include "CountDown.hpp"
#include "../codechiev/BasisUtil.h"

#define NUMBER_SCALE 4

USING_NS_CC;
using namespace codechiev;
CountDown::CountDown():left(0.0f)
{}
bool
CountDown::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    progress_ = ProgressTimer::create(Sprite::create("countdown.png"));
    progress_->setType( ProgressTimer::Type::RADIAL );
    progress_->setPosition(gCenter);
    this->addChild(progress_);
    
    number_ = Label::createWithBMFont("fonts/arial-unicode-26.fnt", "0");
    number_->setPosition(gCenter);
    number_->setScale(NUMBER_SCALE);
    this->addChild(number_);
    
    this->scheduleUpdate();
    return true;
}

void
CountDown::countdownBegin(float repeat)
{
    left = repeat;
    //countdownUpdate(0);
    this->schedule(SEL_SCHEDULE(&CountDown::countdownUpdate), 1);
    setNumber(left);
    auto to = Sequence::createWithTwoActions(ProgressTo::create(repeat, 100), ProgressTo::create(0, 0));
    progress_->runAction(to);
}

void
CountDown::countdownBegin(float repeat, const timeup_callback& cb)
{
    cb_ = cb;
    countdownBegin(repeat);
}
void
CountDown::setNumber(float num)
{
    if(left<=5)
        number_->runAction(Sequence::createWithTwoActions(ScaleBy::create(0.05f, NUMBER_SCALE-.1f),ScaleTo::create(0.125f, NUMBER_SCALE)));
    number_->setString(StringUtils::format("%g",num));
}
void
CountDown::countdownUpdate(float dt)
{
    --left;
    //Animation
    if(left)
    {
        setNumber(left);
    }else if(left==0&&cb_)
    {
        cb_();
    }
    if(left<0)
    {
        this->unschedule(SEL_SCHEDULE(&CountDown::countdownUpdate));
        this->removeFromParent();//destroy here
    }
}

void
CountDown::stop()
{
    left=-1;
    this->setVisible(false);
}

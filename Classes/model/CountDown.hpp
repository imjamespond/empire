//
//  CountDownTimer.hpp
//  empire
//
//  Created by metasoft on 16/4/20.
//
//

#ifndef CountDownTimer_hpp
#define CountDownTimer_hpp

#include <cocos2d.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace codechiev {

class CountDown : public cocos2d::Layer
{
public:
    typedef boost::function<void()> timeup_callback;
    
    CountDown();
    CREATE_FUNC(CountDown);
    
    virtual bool init();

    void countdownBegin(float);
    void countdownBegin(float, const timeup_callback&);
    void setNumber(float );
    void stop();
    
    float left;
private:
    void countdownUpdate(float);
    
    bool stop_;
    cocos2d::Label *number_;
    cocos2d::ProgressTimer *progress_;
    timeup_callback cb_;
};
    
}
#endif /* CountDownTimer_hpp */

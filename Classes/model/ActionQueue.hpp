//
//  ActionQueue.hpp
//  empire
//
//  Created by metasoft on 16/5/5.
//
//

#ifndef ActionQueue_hpp
#define ActionQueue_hpp

#include <cocos2d.h>
#include <boost/function.hpp>
#include <deque>

class ActionQueue
{
public:
    typedef boost::function<void ()> action_func;
    typedef std::deque<action_func > action_queue;
    
    void addQueue(const action_func& func);
    void playNext();
    void playStart();
    
    action_queue actionQueue;
    //action_func done;
};

void inline
ActionQueue::addQueue(const action_func &func)
{
    actionQueue.push_back(func);
}

void inline
ActionQueue::playNext()
{
    cocos2d::log("ActionQueue playNext: %lu", actionQueue.size());
    if(actionQueue.size())
    {
        //pop previous
        actionQueue.pop_front();
    }
    
    if(actionQueue.size())
    {
        action_func& func = actionQueue.front();
        if(func)
        {
            cocos2d::log("ActionQueue func: %lu", actionQueue.size());
            func();
        }
    }
}

void inline
ActionQueue::playStart()
{
    if(actionQueue.size())
    {
        action_func& func = actionQueue.front();
        if(func)
        {
            func();
        }
    }
}

#endif /* ActionQueue_hpp */

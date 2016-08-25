//
//  GameMenuLayer.hpp
//  empire
//
//  Created by metasoft on 16/4/19.
//
//

#ifndef GameMenuLayer_hpp
#define GameMenuLayer_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <external/json/document.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "../model/Game.hpp"

class SwapBtn;
class GameMenuLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameMenuLayer);
    
    virtual bool init();
    
    void initGameMenu();
    void initSwapMenu(cocos2d::Node*);
    void updateHealth(const codechiev::Game::Player&, int, int);
    void showSwapMenu(bool show=true);
    void addSwapMenu(int);
    void expiredEvent(rapidjson::Document &);
    void removeOperBtn(codechiev::Game::Player& ,int);

    //oper_struct blueopers[5];
    //oper_struct redopers[5];
    cocos2d::Sprite* blue;
    cocos2d::Sprite* red;

public:
    void timerBegin(float);
    void timerStop();
    void updateTimer(float);
    void updateTimerText();
    
    codechiev::Game *game;
};

class SwapBtn : public cocos2d::Sprite
{
public:
    CREATE_FUNC(SwapBtn);
    SwapBtn():type(0){}
    virtual bool init();
    inline cocos2d::EventListener* getListener(){return listener;}
    
    int type;
    cocos2d::EventListener* listener;
};

#endif /* GameMenuLayer_hpp */

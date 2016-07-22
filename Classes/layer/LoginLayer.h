#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <external/json/document.h>

class LoginLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(LoginLayer);

	virtual bool init();//to invoke method of subclass
	virtual void onEnter();

	void connectCallback(cocos2d::Ref*);
	void loginBtnClick(cocos2d::Ref*);
	void loginOk(rapidjson::Document&, bool ok);
    void createOk(rapidjson::Document&, bool ok);
    inline cocos2d::Node* getLoginNode(){return loginNode_;}
    
    static void showLogin( bool needPwd=false);
private:
    cocos2d::Node* loginNode_;
    cocos2d::ui::TextField *name_;
    cocos2d::ui::Button *loginBtn_;
    cocos2d::ui::TextField *passwd_;
    //cocos2d::ui::EditBox *editName_;
    //cocos2d::ui::EditBox *editPasswd_;
};
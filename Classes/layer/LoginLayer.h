#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <external/json/document.h>

static const char kLoginLayer[] = "ui/LoginLayer.csb";
static const char kSignupLayer[] = "ui/SignupLayer.csb";

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
    
    static void showLogin( bool needPwd=false);
    
    cocos2d::Layer* layer;
    cocos2d::ui::TextField *name;
    cocos2d::ui::Button *loginBtn;
    cocos2d::ui::TextField *passwd;
private:
    

    //cocos2d::ui::EditBox *editName_;
    //cocos2d::ui::EditBox *editPasswd_;
};
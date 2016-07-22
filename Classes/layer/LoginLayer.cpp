#include "LoginLayer.h"
#include "MenuLayer.h"
#include "GameLayer.h"
#include "GameSceneLayer.h"
#include "NotificationLayer.h"
#include "../model/CardLayer.hpp"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/Network.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/control/LoginController.h"
#include "../codechiev/control/ChatController.hpp"
#include "../layer/MenuLayer.h"
#include "../model/User.h"
#include "../model/MessageLayer.hpp"
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace codechiev;

void
LoginLayer::onEnter()
{
	Layer::onEnter();
    
    //auto login
    std::string username = base::Singleton<BasisUtil>::get()->getAppString(AK_UNAME);
    std::string passwd = base::Singleton<BasisUtil>::get()->getAppString(AK_PASSWD);
    std::string deviceId = base::Singleton<BasisUtil>::get()->getAppString(AK_DEVICE);
    if(username.size()&&passwd.size())
    {
        loginBtn_->setEnabled(false);
        gNotificationLayer->loading();
        
        control::LoginController::login(username, passwd, deviceId, std::bind(&LoginLayer::loginOk, this, std::placeholders::_1, std::placeholders::_2));
    }
}

bool
LoginLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
    loginNode_ = CSLoader::createNode(kLoginLayer);
    loginNode_->setPosition(gCenter);
    
    loginBtn_ = static_cast<ui::Button*>(loginNode_->getChildByName("BTN_LOGIN"));
    loginBtn_->addClickEventListener(CC_CALLBACK_1(LoginLayer::loginBtnClick, this));

    name_ = static_cast<ui::TextField*>(loginNode_->getChildByName("Node_Name")->getChildByName("TF_Name"));
    name_->setTextVerticalAlignment(TextVAlignment::CENTER);
    passwd_ = static_cast<ui::TextField*>(loginNode_->getChildByName("Node_Passwd")->getChildByName("TF_Passwd"));
    passwd_->setTextVerticalAlignment(TextVAlignment::CENTER);
    
    this->addChild(loginNode_);
	return true;
}

void
LoginLayer::showLogin( bool needPwd)
{
    auto loginLayer = LoginLayer::create();
    loginLayer->setPosition(gOrigin);
    gScene->addChild(loginLayer,999);
    
    if(needPwd)
    {
        Node* editNode = loginLayer->getLoginNode()->getChildByName("Node_Passwd");
        editNode->setVisible(true);
    }
}

void
LoginLayer::loginBtnClick(Ref*)
{
    loginBtn_->setEnabled(false);
    gNotificationLayer->loading();
    
    std::string username = name_->getString();
    std::string passwd = passwd_->getString();
    //login
    if(username.size()&&passwd.size())
    {
        base::Singleton<BasisUtil>::get()->setAppString(AK_UNAME, username);
        base::Singleton<BasisUtil>::get()->setAppString(AK_PASSWD, passwd);
        std::string deviceId;
        control::LoginController::login(username, passwd, deviceId, std::bind(&LoginLayer::loginOk, this, std::placeholders::_1, std::placeholders::_2));
    }
    //create without passwd input
    else if(username.size()&&passwd.size()==0)
    {
        passwd = randomPasswd(6);
        base::Singleton<BasisUtil>::get()->setAppString(AK_UNAME, username);
        base::Singleton<BasisUtil>::get()->setAppString(AK_PASSWD, passwd);
        
        control::LoginController::create(username, passwd, std::bind(&LoginLayer::createOk, this, std::placeholders::_1, std::placeholders::_2));
    }

}
void
LoginLayer::createOk(rapidjson::Document& doc, bool ok)
{
    loginOk(doc, ok);
    
    gMenuLayer->cardLayer->onCreateCard();
}
void
LoginLayer::loginOk(rapidjson::Document& doc, bool ok)
{
    gNotificationLayer->quitLoading();
    if(!ok)
    {
        loginBtn_->setEnabled(true);
        return;
    }
    
	if (gMenuLayer)
	{
		gMenuLayer->setVisible(true);
		this->removeFromParent();
        
        const rapidjson::Value& data = doc["data"];
        if(data.IsNull())
            return;
        
        User *self = base::Singleton<User>::get();
        
        const rapidjson::Value& uid = data["uid"];
        if(!uid.IsNull())
            self->uid = uid.GetInt64();
        
        const rapidjson::Value& name = data["name"];
        if(!name.IsNull())
            self->name = name.GetString();
        
        const rapidjson::Value& level = data["level"];
        if(!level.IsNull())
            self->level = level.GetInt();
        
        const rapidjson::Value& score = data["score"];
        if(!score.IsNull())
            self->score = score.GetInt();
        
        EventCustom userEvent(kUserEvent);
        userEvent.setUserData(nullptr);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&userEvent);
        
        const rapidjson::Value& upgrade = data["upgrade"];
        if(!upgrade.IsNull())
            gMenuLayer->cardLayer->updateCardsEvent(upgrade);
        
        control::ChatController::chatList(std::bind(&MessageLayer::chatListOk, gMenuLayer->msgLayer, std::placeholders::_1));
	}
}
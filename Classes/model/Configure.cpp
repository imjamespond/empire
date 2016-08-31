//
//  Configure.cpp
//  empire
//
//  Created by metasoft on 16/4/26.
//
//

#include "Configure.hpp"
#include "AudioManager.h"
#include "HallLayer.hpp"
#include "User.h"
#include "../resource.h"
#include "../layer/GameLayer.h"
#include "../layer/MenuLayer.h"
#include "../layer/LoginLayer.h"
#include "../layer/NotificationLayer.h"
#include "../codechiev/BasisUtil.h"
#include "../codechiev/control/LoginController.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace codechiev;

Configure::Configure():
btnChange(nullptr),
btnQuit(nullptr),
ckMute(nullptr),
editPasswd(nullptr),
slideVolumn(nullptr)
{}

bool
Configure::init()
{
    if (!ModalLayer::init())
    {
        return false;
    }
    
    auto posLayer = CSLoader::createNode(kConfigPosLayer);
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kConfigLayer)));
    
    auto btnService = static_cast<ui::Button*>(modalLayer->getChildByName("Button_1"));
    btnService->addClickEventListener(std::bind(&Configure::onService, this, std::placeholders::_1));
    auto privacyBtn = static_cast<ui::Button*>(modalLayer->getChildByName("Button_2"));
    privacyBtn->addClickEventListener(std::bind([=](){
        Application::getInstance()->openURL(kPrivacyTerm);
    }));
    
    /*auto btnDebug = static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Debug"));
    btnDebug->addClickEventListener(std::bind([=](Ref* ref){
        static bool v(1);
        v^=1;
        gNotificationLayer->debug->setVisible(v);
        gMenuLayer->hallLayer->pageViewMap->setCurrentPageIndex(0);
        gMenuLayer->configLayer->undoModal(gMenuLayer->navLayer, true);
    }, std::placeholders::_1));
    */
    auto btnClose = static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Shut"));
    btnClose->addClickEventListener(std::bind([=](Node* pt) {
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }, this));
    
    auto changePwd = posLayer->getChildByName("BTN_ChangePwd");
    auto confirmPwd = posLayer->getChildByName("BTN_ConfirmPwd");
    ui::RadioButtonGroup *radioGroup = ui::RadioButtonGroup::create();
    addChild(radioGroup);
    radioGroup->setAllowedNoSelection(false);
    radioGroup->addEventListener(CC_CALLBACK_3(Configure::changePasswd, this));
    
    ui::RadioButton* radioConfirmPwd = ui::RadioButton::create(kUIChangePwd, kUIConfirmPwdDisable);
    radioConfirmPwd->setPosition(confirmPwd->getPosition()+gCenter);
    this->addChild(radioConfirmPwd);
    radioGroup->addRadioButton(radioConfirmPwd);
    ui::RadioButton* radioChangePwd = ui::RadioButton::create(kUIChangePwd, kUIChangePwdDisable);
    radioChangePwd->setPosition(changePwd->getPosition()+gCenter);
    this->addChild(radioChangePwd);
    radioGroup->addRadioButton(radioChangePwd);
    
    btnQuit = static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Quit"));
    btnQuit->addClickEventListener(CC_CALLBACK_1(Configure::quit, this));
    
    ckMute = static_cast<ui::CheckBox*>(modalLayer->getChildByName("CK_Mutu"));
    ckMute->addEventListener(std::bind([=](Ref* pSender,ui::CheckBox::EventType type){
        switch (type)
        {
            case ui::CheckBox::EventType::SELECTED:
                AudioManager::play(AD_BACKGROUND);
                base::Singleton<BasisUtil>::get()->setAppBool(AK_MUTE, false);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                AudioManager::stop(AD_BACKGROUND);
                base::Singleton<BasisUtil>::get()->setAppBool(AK_MUTE, true);
                break;
            default:
                break;
        }
        
    }, std::placeholders::_1, std::placeholders::_2));
    //play audio
    if(base::Singleton<BasisUtil>::get()->getAppBool(AK_MUTE))
    {
        ckMute->setSelected(false);
    }
    
    auto account = static_cast<ui::Text*>(modalLayer->getChildByName("Text_Account"));
    account->setString(base::Singleton<User>::get()->name );
    
    auto passwd = static_cast<ui::ImageView*>(posLayer->getChildByName("Passwd"));
    const Size &editPasswdSize = passwd->getContentSize();
    editPasswd = ui::EditBox::create(editPasswdSize, kImgEditor);
    editPasswd->setPosition(passwd->getPosition() + gCenter);
    editPasswd->setFontColor(Color3B::GREEN);
    editPasswd->setPlaceHolder("密码:");
    editPasswd->setMaxLength(6);
    editPasswd->setEnabled(false);
    //editPasswd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    editPasswd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    editPasswd->setFontSize(editPasswdSize.height / 2);
    //_editPassword->setDelegate(this);
    this->addChild(editPasswd);
    
    slideVolumn = static_cast<ui::Slider*>(modalLayer->getChildByName("Slider_Volumn"));
    slideVolumn->setPercent(base::Singleton<BasisUtil>::get()->getAppFloat(AK_VOL)*100);
    slideVolumn->addEventListener(std::bind([=](Ref *pSender, ui::Slider::EventType type){
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(pSender);
            float percent = slider->getPercent();
            float maxPercent = slider->getMaxPercent();
            float volumn(percent/maxPercent);
            AudioManager::setVolumn(volumn);
            base::Singleton<BasisUtil>::get()->setAppFloat(AK_VOL, volumn);
        }
    }, std::placeholders::_1, std::placeholders::_2));
    
    return true;
}

void
Configure::quit(Ref*)
{
    base::Singleton<BasisUtil>::get()->appDataClear();
    LoginLayer::showLogin( true);
}

void
Configure::changePasswd(ui::RadioButton* radioButton, int index, ui::RadioButtonGroup::EventType type)
{
    if(index)
    {
        editPasswd->setEnabled(true);
    }
    else
    {
        base::Singleton<BasisUtil>::get()->setAppString(AK_PASSWD, editPasswd->getText());
        control::LoginController::changePasswd(editPasswd->getText(),
                                               std::bind([=](rapidjson::Document& doc, bool ok) {
            if(!ok)
                return;
            base::Singleton<BasisUtil>::get()->appDataSave();
            gNotificationLayer->onError("密码修改成功");
        }, std::placeholders::_1, std::placeholders::_2));
        
        editPasswd->setEnabled(false);
    }
}

void
Configure::onService(cocos2d::Ref *)
{
    auto layer = ServiceLayer::create();
    layer->setVisible(false);
    gMenuLayer->addChild(layer,999);
    layer->doModal();
}


bool
ServiceLayer::init()
{
    if(!ModalLayer::init())
    {
        return false;
    }
    
    this->initModal( static_cast<Layer*>(CSLoader::createNode(kServiceLayer)));
    modalLayer->setPosition(Vec2::ZERO);
    
    auto close=static_cast<ui::Button*>(modalLayer->getChildByName("BTN_Close"));
    close->addClickEventListener(std::bind(&ModalLayer::onClose, this, std::placeholders::_1));
    
    return true;
}
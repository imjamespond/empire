//
//  Configure.hpp
//  empire
//
//  Created by metasoft on 16/4/26.
//
//

#ifndef Configure_hpp
#define Configure_hpp

#include "ModalLayer.hpp"
#include <cocos2d.h>
#include <ui/CocosGUI.h>

static const char kConfigPosLayer[] = "ui/ConfigPosLayer.csb";
static const char kServiceLayer[] = "ui/ServiceLayer.csb";

namespace codechiev
{
    class Configure : public ModalLayer
    {
    public:
        Configure();
        
        CREATE_FUNC(Configure);
        
        virtual bool init();
        
        void changePasswd(cocos2d::ui::RadioButton*, int, cocos2d::ui::RadioButtonGroup::EventType);
        void quit(cocos2d::Ref* );
        void onService(cocos2d::Ref *);
        
        cocos2d::ui::Button* btnChange;
        cocos2d::ui::Button* btnQuit;
        cocos2d::ui::CheckBox* ckMute;
        cocos2d::ui::EditBox* editPasswd;
        cocos2d::ui::Slider* slideVolumn;
        
    private:
        
    };
    
    class ServiceLayer : public ModalLayer
    {
    public:
        CREATE_FUNC(ServiceLayer);
        
        virtual bool init();
    };
}

#endif /* Configure_hpp */

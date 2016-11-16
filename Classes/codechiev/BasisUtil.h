#pragma once
#include <string>
#include <deque>
#include <cocos2d.h>
#include "base/singleton.h"
#include "base/rsa.h"
#include "base/hash.hpp"

extern cocos2d::Vec2 gVisibleSize;
extern cocos2d::Vec2 gOrigin;

extern cocos2d::Vec2 gRightBottom;
extern cocos2d::Vec2 gCenter;
extern cocos2d::Vec2 gLeftBottom;
extern cocos2d::Vec2 gLeftTop;

namespace codechiev
{
    enum AppKey
    {
        AK_UNAME,
        AK_PASSWD,
        AK_DEVICE,
        AK_MUTE,
        AK_VOL,
        Ak_CARD_POS,
        AK_GUIDE_PASSED,
        AK_THUMBMAP,
        AK_TOTAL_SIZE,
    };
    static const char* kAppKeys[AK_TOTAL_SIZE] =
    {
        "username",
        "passwd",
        "deviceId",
        "mute",
        "volumn",
        "cardposition",
        "guidepassed",
        "thumbmap"
    };
    
	class BasisUtil
	{
	public:
		BasisUtil();
		~BasisUtil();

        void setAppString(AppKey, const std::string&);
        void setAppBool(AppKey, bool);
        void setAppFloat(AppKey, float);
        std::string getAppString(AppKey);
        bool getAppBool(AppKey);
        float getAppFloat(AppKey);
        
		void appData();
		void appDataSave();
        void appDataClear();

		void init();
	private:
		//cocos2d::ValueMap valMap_;
	};

	class BaseDebug
	{
	public:
		BaseDebug();
		~BaseDebug();

		void add(const std::string&);
		std::string toString();
		bool dirty;
	private:
		std::deque<std::string> msgDeq_;
	};

    extern std::string getUuid();
	extern std::string getDeviceID();
    extern std::string randomPasswd(int );
    
    extern void setRightTop(cocos2d::Node* , float );
    extern void setRightBottom(cocos2d::Node* , float );
    extern void setCenterBottom(cocos2d::Node* , float );
    extern void showElasticDialog(cocos2d::Layer*, float time=.5f);
    
    extern void showKeyboard();
    extern void onDismissKeyboard();
    
    extern void copyData(const char* );
    extern bool isFileExist(const char* );
}


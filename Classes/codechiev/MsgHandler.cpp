#include "MsgHandler.h"
#include "../layer/NotificationLayer.h"
#include "BaseCmd.h"
#include "BasisUtil.h"

using namespace codechiev;

MsgHandler::MsgHandler(const char* msg):jsonDoc_(), error_(false)
{
	error_ = jsonDoc_.Parse(msg).HasParseError();

	const rapidjson::Value& ok = jsonDoc_["ok"];
	if (!ok.IsNull())
	{
		if (!ok.IsTrue())
        {
            const rapidjson::Value& event = jsonDoc_["event"];
            if (!event.IsNull() && event.IsString())
            {
                if (::strcmp(event.GetString(), "event.error") == 0)
                {
                    const rapidjson::Value& data = jsonDoc_["data"];
                    if (!data.IsNull() && data.IsString())
                    {
                        gNotificationLayer->onError(data.GetString());
                    }
                }
            }
        }
        
        const rapidjson::Value& serial = jsonDoc_["serial"];
        if (!serial.IsNull() && serial.IsInt() && serial.GetInt()>0)
        {
            codechiev::base::Singleton<CmdCallback>::get()->invokeCallback(serial.GetInt(), jsonDoc_, ok.IsTrue());
            return;
        }
        const rapidjson::Value& event = jsonDoc_["event"];
        if (!event.IsNull() && event.IsString())
        {
            codechiev::base::Singleton<CmdCallback>::get()->handleEvent(event.GetString(), jsonDoc_);
            return;
        }
        cocos2d::log("MsgHandler serial error");
	}

	cocos2d::log("MsgHandler ok error");
}

MsgHandler::~MsgHandler()
{
}

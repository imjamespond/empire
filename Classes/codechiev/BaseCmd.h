#pragma once

#include <string>
#include <external/json/document.h>     // rapidjson's DOM-style API
#include <external/json/prettywriter.h> // for stringify JSON
#include <functional>
#include <unordered_map>
#include "CmdConfig.h"

namespace codechiev
{
#define CmdCallbackBegin std::bind([=](rapidjson::Document& doc, bool ok){
#define CmdCallbackBeginWithData std::bind([=](rapidjson::Document& doc, bool ok){\
    const rapidjson::Value& data = doc["data"];
#define CmdCallbackEnd },std::placeholders::_1, std::placeholders::_2)

	class CmdCallback
	{
	public:
		typedef std::function<void(rapidjson::Document&, bool)> callback;
		typedef std::unordered_map<int, callback> callback_map;
        typedef std::unordered_map<std::string, callback> event_map;

		inline void registCallback(int serial, const callback& cb)
        { callbackMap_[serial] = cb; }
        inline void registEvent(const std::string& event, const callback& cb)
        { eventMap_[event] = cb; }
        inline void eraseEvent(const std::string& event)
        { eventMap_.erase(event); }
		void invokeCallback(int , rapidjson::Document&, bool ok=true );
        void handleEvent(const std::string& , rapidjson::Document&, bool ok=true );
	private:
		callback_map callbackMap_;
        event_map eventMap_;
	};

	class BaseCmd
	{
	public:
		explicit BaseCmd(cmd::CmdSerial, const CmdCallback::callback&);
		~BaseCmd();

		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer;
	private:
		rapidjson::StringBuffer sb_;

		int serial_;
	};

}



#include "BaseCmd.h"

#include "MsgHandler.h"
#include "Network.h"
#include "base/singleton.h"

using namespace codechiev;
using namespace rapidjson;

void 
CmdCallback::invokeCallback(int serial, rapidjson::Document& doc, bool ok)
{ 
	callback cb = callbackMap_[serial]; 
	if (cb) 
		cb(doc, ok);
	callbackMap_.erase(serial);
}

void
CmdCallback::handleEvent(const std::string & event, rapidjson::Document &doc, bool ok)
{
    callback cb = eventMap_[event];
    if (cb)
        cb(doc, ok);
}

BaseCmd::BaseCmd(cmd::CmdSerial s, const CmdCallback::callback& cb):
	serial_(-1), sb_(), writer(sb_)
{
	assert(s<cmd::CmdSerialSize);

	serial_ = s;
    codechiev::base::Singleton<CmdCallback>::get()->registCallback(serial_, cb);

	writer.StartObject();

	writer.Key("serial");
	writer.Uint(serial_);

	writer.Key("cmd");
	writer.String(cmd::gCommands[serial_].cmd);

	writer.Key("type");
	writer.String(cmd::gCommands[serial_].obj);

	writer.Key("param");
	writer.StartObject();
}


BaseCmd::~BaseCmd()
{
	writer.EndObject();
	writer.EndObject();

    base::Singleton<Network>::get()->send(sb_.GetString());
}

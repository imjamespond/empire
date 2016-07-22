//
//  ChatController.cpp
//  empire
//
//  Created by metasoft on 16/4/25.
//
//

#include "ChatController.hpp"
#include "../BasisUtil.h"

using namespace codechiev;
using namespace codechiev::control;

void
ChatController::chat(const std::string & msg, const CmdCallback::callback & cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdGameChat, cb);
    cmd.writer.Key("name");
    cmd.writer.String("");
    cmd.writer.Key("msg");
    cmd.writer.String(msg.c_str());
    cmd.writer.Key("type");
    cmd.writer.Int(1);
}

void
ChatController::chatList( const CmdCallback::callback & cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdGameChatList, cb);
}
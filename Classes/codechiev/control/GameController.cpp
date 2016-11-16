//
//  GameController.cpp
//  empire
//
//  Created by metasoft on 16/4/25.
//
//

#include "GameController.hpp"
#include "../BasisUtil.h"

using namespace codechiev;
using namespace codechiev::control;

void
GameController::allocate(int map, const roles_vec& roles, const CmdCallback::callback &cb)
{
    BaseCmd cmd(cmd::CmdGameAllocate, cb);
    
    cmd.writer.Key("map");
    cmd.writer.Int(map);
    
    cmd.writer.Key("roles");
    cmd.writer.StartArray();
    for( roles_vec::const_iterator it=roles.begin();
        it!=roles.end();
        it++)
    {
        cmd.writer.Int(*it);
    }
    
    cmd.writer.EndArray();
}

void
GameController::pveAlloc(int map, const roles_vec & roles, const CmdCallback::callback &cb)
{
    BaseCmd cmd(cmd::CmdGamePveAlloc, cb);
    
    cmd.writer.Key("map");
    cmd.writer.Int(map);
    
    cmd.writer.Key("roles");
    cmd.writer.StartArray();
    for( roles_vec::const_iterator it=roles.begin();
        it!=roles.end();
        it++)
    {
        cmd.writer.Int(*it);
    }
    
    cmd.writer.EndArray();
}

void
GameController::swap(int type, const CmdCallback::callback &cb)
{
    BaseCmd cmd(cmd::CmdGameSwap, cb);
    
    cmd.writer.Key("type");
    cmd.writer.Int(type);
}

void
GameController::exit(const CmdCallback::callback & cb)
{
    BaseCmd cmd(cmd::CmdGameExit, cb);
}

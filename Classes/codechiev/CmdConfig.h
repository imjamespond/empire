#pragma once
#include <string>

namespace codechiev

{

	namespace cmd

	{
        struct Command
        {
            Command(const char* o,const char* c):obj(o),cmd(c){}
            const char* obj;
            const char* cmd;
        };

		enum CmdSerial
		{
			CmdError,
			CmdGameAccept,
			CmdGameSwap,
			CmdGameAllocate,
			CmdGamePveAlloc,
			CmdGameInvite,
			CmdUserCreate,
			CmdGameChat,
			CmdUserPing,
			CmdUserLogin,
			CmdUserChange,
			CmdUserRank,
			CmdUserRankRedeem,
			CmdUserUpgrade,
			CmdUserReset,
			CmdUserRecieve,
			CmdGameFangPiao,
			CmdGameRecvFangPiao,
			CmdUserExchange,
			CmdGameChatList,
			CmdSerialSize,
		};
        
        static struct Command gCommands[CmdSerialSize] =
        {
        	Command("com.error" , "error")
			,Command("com.metasoft.empire.vo.GameRequest" , "GameAccept")
			,Command("com.metasoft.empire.vo.GameSwapRequest" , "GameSwap")
			,Command("com.metasoft.empire.vo.GameRequest" , "GameAllocate")
			,Command("com.metasoft.empire.vo.GameRequest" , "GamePveAlloc")
			,Command("com.metasoft.empire.vo.ChatReqInvite" , "GameInvite")
			,Command("com.metasoft.empire.vo.LoginRequest" , "UserCreate")
			,Command("com.metasoft.empire.vo.ChatRequest" , "GameChat")
			,Command("com.metasoft.empire.vo.VoidRequest" , "UserPing")
			,Command("com.metasoft.empire.vo.LoginRequest" , "UserLogin")
			,Command("com.metasoft.empire.vo.LoginRequest" , "UserChange")
			,Command("com.metasoft.empire.vo.RankRequest" , "UserRank")
			,Command("com.metasoft.empire.vo.RankRequest" , "UserRankRedeem")
			,Command("com.metasoft.empire.vo.UpgradeRequest" , "UserUpgrade")
			,Command("com.metasoft.empire.vo.VoidRequest" , "UserReset")
			,Command("com.metasoft.empire.vo.VoidRequest" , "UserRecieve")
			,Command("com.metasoft.empire.vo.ChatReqFangPiao" , "GameFangPiao")
			,Command("com.metasoft.empire.vo.RedeemRecvReq" , "GameRecvFangPiao")
			,Command("com.metasoft.empire.vo.ExchangeReq" , "UserExchange")
			,Command("com.metasoft.empire.vo.VoidRequest" , "GameChatList")
        };

	}

}

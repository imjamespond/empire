#pragma once

#include <string>
#include "../BaseCmd.h"

namespace codechiev
{
	namespace control
	{

		class LoginController
		{
		public:
            typedef std::vector<int> roles_vec;
            static void ping(const CmdCallback::callback&);
			static void login(const std::string&, const std::string&, std::string&, const CmdCallback::callback&);
            static void create(const std::string&, const std::string&, const CmdCallback::callback&);
            static void changePasswd(const std::string&, const CmdCallback::callback&);
            static void rankList(int, int, const CmdCallback::callback&);
            static void rankRedeem(int, int, const CmdCallback::callback&);
            static void upgrade(int, int, const CmdCallback::callback&);
            static void recieve(const CmdCallback::callback&);
            static void redeem(int , const CmdCallback::callback &);
            static void redeemRecv(std::string , const CmdCallback::callback &);
            static void bufuRecv(std::string ,int , const roles_vec& , const CmdCallback::callback &);
            static void exchange(int , const CmdCallback::callback &);
        };
	}
}




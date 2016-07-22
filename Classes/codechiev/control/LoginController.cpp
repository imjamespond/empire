#include "LoginController.h"
#include "../BasisUtil.h"

using namespace codechiev;
using namespace codechiev::control;

void
LoginController::ping(const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserPing, cb);
}

void
LoginController::create(const std::string& username, const std::string& passwd, const CmdCallback::callback& cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserCreate, cb);
    
    cmd.writer.Key("username");
    cmd.writer.String(username.c_str());
    cmd.writer.Key("password");
    base::Base64::unsignedchar_vec encrytedPasswd;
    int encryptLength  = base::Singleton<base::RsaUtil>::get()->publicPemEncrypt(passwd, encrytedPasswd);
    std::string base64Passwd = base::Base64::Base64Encode(encrytedPasswd.data(), encryptLength);
    cmd.writer.String(base64Passwd.c_str());
    
    cmd.writer.Key("deviceId");
    std::string ime = base::Singleton<BasisUtil>::get()->getAppString(AK_DEVICE);
    if (ime.size() == 0)
    {
        ime = getDeviceID();
        if(ime.size() == 0)
        {
            ime = getUuid();
        }
        base::Singleton<BasisUtil>::get()->setAppString(AK_DEVICE, ime);
        cmd.writer.String(ime.c_str());
    }
    else
    {
        cmd.writer.String(ime.c_str());
    }
    
    /*//test decoding
     base::Base64::unsignedchar_vec base64decodeData;
     base::Base64::Base64Decode(base64Passwd.c_str(), base64decodeData);
     cocos2d::log(cocos2d::StringUtils::format("%s,%lu", base64Passwd.c_str(), base64decodeData.size()).c_str());
     
     base::Base64::unsignedchar_vec decryptedPasswd;
     base::Singleton<base::RsaUtil>::get()->privatePemDecrypt(encrytedPasswd.data(), encryptLength, decryptedPasswd);
     cocos2d::log(reinterpret_cast<const char*>(decryptedPasswd.data()));
     
     gBasisUtil.setAppUsername(username.c_str());*/
}
void
LoginController::login(const std::string& username, const std::string& password, std::string& ime, const CmdCallback::callback& cb)
{
	codechiev::BaseCmd cmd(codechiev::cmd::CmdUserLogin, cb);

	cmd.writer.Key("username");
	cmd.writer.String(username.c_str());
	cmd.writer.Key("password");
    std::string shaPasswd = base::Hash::sha256(password);
    cmd.writer.String(shaPasswd.c_str());

	cmd.writer.Key("deviceId");
    if (ime.size() == 0)
    {
        ime = getDeviceID();
        if(ime.size() == 0)
        {
            ime = getUuid();
        }
        base::Singleton<BasisUtil>::get()->setAppString(AK_DEVICE, ime);
        cmd.writer.String(ime.c_str());
    }
    else
    {
        cmd.writer.String(ime.c_str());
    }
}

void
LoginController::changePasswd(const std::string &passwd, const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserChange, cb);
    cmd.writer.Key("username");
    cmd.writer.Null();
    
    cmd.writer.Key("password");
    base::Base64::unsignedchar_vec encrytedPasswd;
    int encryptLength  = base::Singleton<base::RsaUtil>::get()->publicPemEncrypt(passwd, encrytedPasswd);
    std::string base64Passwd = base::Base64::Base64Encode(encrytedPasswd.data(), encryptLength);
    cmd.writer.String(base64Passwd.c_str());
    
    cmd.writer.Key("deviceId");
    cmd.writer.Null();
}

void
LoginController::rankList(int pgnum, int pgsize, const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserRank, cb);
    cmd.writer.Key("offset");
    cmd.writer.Int(pgnum);
    
    cmd.writer.Key("size");
    cmd.writer.Int(pgsize);
}
void
LoginController::rankRedeem(int pgnum, int pgsize, const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserRankRedeem, cb);
    cmd.writer.Key("offset");
    cmd.writer.Int(pgnum);
    
    cmd.writer.Key("size");
    cmd.writer.Int(pgsize);
}

void
LoginController::upgrade(int roleid, int num, const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserUpgrade, cb);
    cmd.writer.Key("id");
    cmd.writer.Int(roleid);
    cmd.writer.Key("num");
    cmd.writer.Int(num);
}

void
LoginController::recieve(const CmdCallback::callback & cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserRecieve, cb);
}

void
LoginController::redeem(int roleid, const CmdCallback::callback & cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdGameFangPiao, cb);
    
    cmd.writer.Key("roleid");
    cmd.writer.Int(roleid);
    cmd.writer.Key("num");
    cmd.writer.Int(1);
}

void
LoginController::redeemRecv(std::string key, const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdGameRecvFangPiao, cb);
    
    cmd.writer.Key("key");
    cmd.writer.String(key.c_str());
}

void
LoginController::exchange(int score, const CmdCallback::callback &cb)
{
    codechiev::BaseCmd cmd(codechiev::cmd::CmdUserExchange, cb);
    
    cmd.writer.Key("score");
    cmd.writer.Int(score);
}


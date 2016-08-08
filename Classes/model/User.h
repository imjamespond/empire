//
//  User.h
//  empire
//
//  Created by metasoft on 16/5/11.
//
//

#ifndef User_h
#define User_h
#include <string>
namespace codechiev
{
    static const char kUserEvent[] = "user.event";
    
    struct User
    {
        User():uid(0),level(0),score(0),redeem(0),recieve(0){}
        int64_t uid;
        std::string name;
        int level;
        int score;
        int redeem;
        int recieve;
    };
}


#endif /* User_h */

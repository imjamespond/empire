//
//  ChatController.hpp
//  empire
//
//  Created by metasoft on 16/4/25.
//
//

#ifndef ChatController_hpp
#define ChatController_hpp

#include <string>
#include "../BaseCmd.h"

namespace codechiev
{
    namespace control
    {
        
        class ChatController
        {
        public:
            
            static void chat(const std::string&, const CmdCallback::callback&);
            static void chatList( const CmdCallback::callback&);
        };
    }
}

#endif /* ChatController_hpp */

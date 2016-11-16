//
//  GameController.hpp
//  empire
//
//  Created by metasoft on 16/4/25.
//
//

#ifndef GameController_hpp
#define GameController_hpp

#include <string>
#include <vector>
#include "../BaseCmd.h"

namespace codechiev
{
    namespace control
    {
        
        class GameController
        {
        public:
            typedef std::vector<int> roles_vec;
            static void allocate(int, const roles_vec&, const CmdCallback::callback&);
            static void pveAlloc(int, const roles_vec&, const CmdCallback::callback&);
            static void swap(int , const CmdCallback::callback&);
            static void exit(const CmdCallback::callback&);
        };
    }
}

#endif /* GameController_hpp */

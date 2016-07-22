//
//  sha.hpp
//  empire
//
//  Created by metasoft on 16/4/12.
//
//

#ifndef sha_hpp
#define sha_hpp

#include <string>
#include <vector>

namespace codechiev
{
    namespace base
    {
        class Hash
        {
        public:
            typedef std::vector<unsigned char> unsignedchar_vec;
            
            Hash();
            ~Hash();
            
            static std::string sha256(const std::string&);
            
        protected:
        private:
        };
    }
}

#endif /* sha_hpp */

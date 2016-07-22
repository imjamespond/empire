//
//  base64.hpp
//  empire
//
//  Created by metasoft on 16/4/8.
//
//

#ifndef base64_hpp
#define base64_hpp

#include <string>
#include <vector>

namespace codechiev
{
    namespace base
    {
class Base64
{
public:
    typedef std::vector<unsigned char> unsignedchar_vec;
    
    Base64();
    ~Base64();
    
    std::string encode(const std::string&);
    std::string decode(const std::string&);
    static std::string Base64Encode(const unsigned char* , size_t );
    static int Base64Decode(const char* , unsignedchar_vec& );
    
protected:
private:
};
    }
}
#endif /* base64_hpp */

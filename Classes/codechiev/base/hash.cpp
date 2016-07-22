//
//  sha.cpp
//  empire
//
//  Created by metasoft on 16/4/12.
//
//

#include "hash.hpp"
#include <sstream>
#include <string.h>
#include <exception>
#include <openssl/evp.h>
#include <stdlib.h>

using namespace codechiev::base;
Hash::Hash()
{
    //ctor
    throw std::exception();
}

std::string
Hash::sha256(const std::string &str)
{
    EVP_MD_CTX *mdctx;
    unsigned char md_value[EVP_MAX_MD_SIZE] = {};
    unsigned int md_len;
    //OpenSSL_add_all_digests();
    
    const EVP_MD *md = EVP_sha256();
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, str.c_str(), strlen(str.c_str()));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);
    
    std::stringstream ss;
    for(int i=0; i<md_len; ++i)
    {
        ss << std::hex << (int)md_value[i];
    }
    return ss.str();
}

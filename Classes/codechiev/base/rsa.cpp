//
//  rsa.cpp
//  test
//
//  Created by metasoft on 16/4/7.
//  Copyright © 2016年 metasoft. All rights reserved.
//
#include "rsa.h"
#include "../BasisUtil.h"
#include <cocos2d.h>

RSA * createRSAWithFilename(const char * filename,int pub)
{
    FILE * fp = fopen(filename,"rb");
    
    if(fp == NULL)
    {
        cocos2d::log("Unable to open file %s \n",filename);
        return NULL;
    }
    RSA *rsa= RSA_new() ;
    
    if(pub)
    {
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);
    }
    
    return rsa;
}
RSA * createRSA(unsigned char * key,int pub)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        cocos2d::log( "Failed to create key BIO");
        return 0;
    }
    if(pub)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        cocos2d::log( "Failed to create RSA");
    }
    
    return rsa;
}

int padding = RSA_PKCS1_PADDING;
using namespace codechiev::base;
RsaUtil::RsaUtil():pubRSA_(nullptr),priRSA_(nullptr)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(!codechiev::isFileExist(publicPem))
        codechiev::copyData(publicPem);
    cocos2d::FileUtils* fu = cocos2d::FileUtils::getInstance();
    std::string pemPath = fu->getWritablePath();
    pemPath += publicPem;
#else
    std::string pemPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(publicPem);
#endif
    pubRSA_ = createRSAWithFilename(pemPath.c_str(),1);
    //std::string priPemPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(privatePem);
    //priRSA_ = createRSAWithFilename(priPemPath.c_str(),0);
}

RsaUtil::~RsaUtil()
{
    if(pubRSA_)
        RSA_free(pubRSA_);
    if(priRSA_)
        RSA_free(priRSA_);
}

int
RsaUtil::publicPemEncrypt(const std::string& data, Base64::unsignedchar_vec& encryptedVec)
{
    unsigned char encrypted[256]={};
    int data_len = static_cast<int>(data.size());
    int length = RSA_public_encrypt(data_len, reinterpret_cast<const unsigned char*>(data.c_str()), encrypted, pubRSA_, padding);
    std::copy(encrypted, encrypted+length, std::back_inserter(encryptedVec));
    return length;
}

int
RsaUtil::privatePemDecrypt(const unsigned char* data, int dataLen, Base64::unsignedchar_vec& decryptedVec)
{
    unsigned char decrypted[256]={};
    int  length = RSA_private_decrypt(dataLen, data, decrypted, priRSA_, padding);
    std::copy(decrypted, decrypted+length, std::back_inserter(decryptedVec));
    decryptedVec.push_back('\0');
    return length;
}
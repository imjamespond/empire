//
//  rsa.h
//  empire
//
//  Created by metasoft on 16/4/7.
//
//

#ifndef rsa_h
#define rsa_h

#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>

#include <assert.h>
#include "base64.hpp"

namespace codechiev
{
    namespace base
    {
        static const char publicPem[] = "public.pem";
        static const char privatePem[] = "pkcs8_priv.pem";
        
        class RsaUtil
        {
        public:
            RsaUtil();
            ~RsaUtil();
            
            int publicPemEncrypt(const std::string&, Base64::unsignedchar_vec& );
            int privatePemDecrypt(const unsigned char*, int, Base64::unsignedchar_vec& );
        private:
            RSA* pubRSA_;
            RSA* priRSA_;
        };
    }
}

#endif /* rsa_h */

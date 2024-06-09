//
// Created by 38491 on 2024/4/7.
//

#ifndef SERVER_DDZ_RSACRYPTO_H
#define SERVER_DDZ_RSACRYPTO_H


#include <string>
#include <openssl/evp.h>
#include <map>
#include "Hash.h"

class RsaCrypto {
public:
    using hashFunc = const EVP_MD *(*)();

    enum KeyLength {
        BITS_1K = 1024,
        BITS_2K = 2048,
        BITS_3K = 3072,
        BITS_4K = 4096,
    };

    enum KeyType {
        PublicKey,
        PrivateKey,
    };

    explicit RsaCrypto() = default;

    // 从密钥文件 加载到 publicKey_/privateKey_
    explicit RsaCrypto(std::string filename, KeyType type);

    ~RsaCrypto();

    // 从 data(内存) 加载到 publicKey_/privateKey_
    void parseStringToKey(std::string data, KeyType type);


    // 生成非对称加密的密钥对，两个文件分别存储公钥和密钥
    void generateRsaKey(KeyLength keyLength, const std::string& pub = "public.pem", const std::string& pri = "private.pem");

    // 通过 公钥 对 对称加密的密钥 进行加密
    std::string pubKeyEncrypt(std::string data);

    // 通过 私钥 对 对称加密的密钥 进行解密
    std::string priKeyDecrypt(std::string data);

    // 数据签名: 通过 私钥 对 数据的哈希值 进行加密
    std::string sign(std::string data, HashType hash = HashType::Sha256);

    // 签名校验: 通过 公钥 对 签证 进行解密，并对数据求哈希值与签证里的哈希对比
    bool varify(std::string sign, std::string data, HashType hash = HashType::Sha256);

private:
    EVP_PKEY *publicKey_{NULL};
    EVP_PKEY *privateKey_{NULL};
};


#endif //SERVER_DDZ_RSACRYPTO_H

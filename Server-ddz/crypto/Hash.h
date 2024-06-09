//
// Created by 38491 on 2024/4/7.
//

#ifndef SERVER_DDZ_HASH_H
#define SERVER_DDZ_HASH_H

#include <openssl/evp.h>
#include <string>
#include <map>

enum class HashType : char {
    Md4,
    Md5,
    Sha1,
    Sha224,
    Sha256,
    Sha384,
    Sha512,
    Sha3_224,
    Sha3_256,
    Sha3_384,
    Sha3_512
};

using hashFunc = const EVP_MD *(*)(void);

const std::map<HashType, hashFunc> hashFunction_{
        {HashType::Md4,      EVP_md4},
        {HashType::Md5,      EVP_md5},
        {HashType::Sha1,     EVP_sha1},
        {HashType::Sha224,   EVP_sha224},
        {HashType::Sha256,   EVP_sha256},
        {HashType::Sha384,   EVP_sha384},
        {HashType::Sha512,   EVP_sha512},
        {HashType::Sha3_224, EVP_sha3_224},
        {HashType::Sha3_256, EVP_sha3_256},
        {HashType::Sha3_384, EVP_sha3_384},
        {HashType::Sha3_512, EVP_sha3_512}
};

class Hash {
public:
    enum class Type : char {
        Binary,
        Hex
    };

    Hash(HashType type);

    ~Hash();

    void addData(std::string data);

    void addData(const char *data, int length);

    std::string result(Type type = Type::Hex);

private:
    EVP_MD_CTX *ctx_{nullptr};
};

#endif //SERVER_DDZ_HASH_H

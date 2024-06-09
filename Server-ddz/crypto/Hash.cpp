//
// Created by 38491 on 2024/4/7.
//

#include <cassert>
#include "Hash.h"

Hash::Hash(HashType type) {
    ctx_ = EVP_MD_CTX_new();
    assert(ctx_ != NULL);
    int ret = EVP_DigestInit_ex(ctx_, hashFunction_.at(type)(), NULL);
    assert(ret == 1);
}

Hash::~Hash() {
    if (ctx_ != NULL) {
        EVP_MD_CTX_free(ctx_);
    }
}

void Hash::addData(std::string data) {
    addData(data.data(), data.size());
}

void Hash::addData(const char *data, int length) {
    int ret = EVP_DigestUpdate(ctx_, data, length);
    assert(ret == 1);
}

std::string Hash::result(Hash::Type type) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    int ret = EVP_DigestFinal_ex(ctx_, digest, &digest_len);
    assert(ret == 1);

    if (type == Type::Hex) {
        char res[digest_len * 2];
        for (int i = 0; i < digest_len; i++) {
            sprintf(&res[i * 2], "%02x", digest[i]);
        }
        return std::string(res, digest_len * 2);
    }

    return std::string(reinterpret_cast<char *>(digest), digest_len);
}

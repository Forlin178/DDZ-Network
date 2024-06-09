//
// Created by 38491 on 2024/4/7.
//

#include "RsaCrypto.h"
#include "Base64.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <cassert>

RsaCrypto::RsaCrypto(std::string filename, KeyType type) {
    BIO *bio = BIO_new_file(filename.data(), "rb");
    assert(bio != NULL);
    if (type == PublicKey) {
        PEM_read_bio_PUBKEY(bio, &publicKey_, NULL, NULL);
    } else {
        PEM_read_bio_PrivateKey(bio, &privateKey_, NULL, NULL);
    }
    BIO_free(bio);
}

RsaCrypto::~RsaCrypto() {
    if (publicKey_)
        EVP_PKEY_free(publicKey_);
    if (privateKey_)
        EVP_PKEY_free(privateKey_);
}

void RsaCrypto::parseStringToKey(std::string data, KeyType type) {
    BIO *bio = BIO_new_mem_buf(data.data(), data.size());
    assert(bio != NULL);
    if (type == PublicKey) {
        PEM_read_bio_PUBKEY(bio, &publicKey_, NULL, NULL);
    } else if (type == PrivateKey) {
        PEM_read_bio_PrivateKey(bio, &privateKey_, NULL, NULL);
    }
    BIO_free(bio);
}

void RsaCrypto::generateRsaKey(KeyLength keyLength, const std::string& pub, const std::string& pri) {
    // 创建上下文ctx
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    // 初始化ctx
    int ret = EVP_PKEY_keygen_init(ctx);
    assert(ret == 1);
    // 指定非对称加密的密钥对的长度
    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, keyLength);
    assert(ret == 1);
    // 生成密钥对
    // EVP_PKEY *key;
    ret = EVP_PKEY_generate(ctx, &privateKey_);
    assert(ret == 1);
    // 释放ctx
    EVP_PKEY_CTX_free(ctx);

    // 将密钥写入磁盘文件
    BIO *bio = BIO_new_file(pri.data(), "wb");
    ret = PEM_write_bio_PrivateKey(bio, privateKey_, NULL, NULL, 0, NULL, NULL);
    assert(ret > 0);
    BIO_flush(bio);
    BIO_free(bio);

    bio = BIO_new_file(pub.data(), "wb");
    ret = PEM_write_bio_PUBKEY(bio, privateKey_);
    assert(ret > 0);
    BIO_flush(bio);
    BIO_free(bio);
}

std::string RsaCrypto::pubKeyEncrypt(std::string data) {
    // 创建上下文
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(publicKey_, NULL);
    assert(ctx != NULL);
    // 初始化上下文
    int ret = EVP_PKEY_encrypt_init(ctx);
    assert(ret == 1);
    // 设置填充方式
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);

    // 使用公钥加密
    size_t outLen;
    EVP_PKEY_encrypt(ctx, NULL, &outLen,
                     reinterpret_cast<const unsigned char *>(data.data()), data.size());
    auto *out = new unsigned char[outLen];
    ret = EVP_PKEY_encrypt(ctx, out, &outLen,
                           reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);

    // 编码为 Base64 格式
    std::string res = Base64::encode(reinterpret_cast<char *>(out), outLen);
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return res;
}

std::string RsaCrypto::priKeyDecrypt(std::string data) {
    // 创建上下文
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privateKey_, NULL);
    assert(ctx != NULL);
    // 初始化上下文
    int ret = EVP_PKEY_decrypt_init(ctx);
    assert(ret == 1);
    // 设置填充方式
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);

    // 解码 Base64 格式
    data = Base64::decode(data);
    // 使用私钥解密
    size_t outLen;
    EVP_PKEY_decrypt(ctx, NULL, &outLen,
                     reinterpret_cast<const unsigned char *>(data.data()), data.size());
    auto *out = new unsigned char[outLen];
    ret = EVP_PKEY_decrypt(ctx, out, &outLen,
                           reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);

    std::string res(reinterpret_cast<char *>(out), outLen);
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return res;
}

std::string RsaCrypto::sign(std::string data, HashType hash) {
    // 计算哈希值
    Hash h(hash);
    h.addData(data);
    std::string hashResult = h.result(Hash::Type::Binary);

    // 创建上下文
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privateKey_, NULL);
    assert(ctx != NULL);
    // 初始化上下文
    int ret = EVP_PKEY_sign_init(ctx);
    assert(ret == 1);
    // 设置填充方式
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);
    // 设置签名算法的哈希函数
    ret = EVP_PKEY_CTX_set_signature_md(ctx, hashFunction_.at(hash)());
    assert(ret == 1);

    // 签名
    size_t sigLen;
    EVP_PKEY_sign(ctx, NULL, &sigLen,
                  reinterpret_cast<const unsigned char *>(hashResult.data()), hashResult.size());
    unsigned char *sig = new unsigned char[sigLen];
    ret = EVP_PKEY_sign(ctx, sig, &sigLen,
                        reinterpret_cast<const unsigned char *>(hashResult.data()), hashResult.size());
    assert(ret == 1);

    std::string res = Base64::encode(reinterpret_cast<char *>(sig), sigLen);
    delete[]sig;
    EVP_PKEY_CTX_free(ctx);
    return res;
}

bool RsaCrypto::varify(std::string sign, std::string data, HashType hash) {
    sign = Base64::decode(sign);

    // 计算哈希值
    Hash h(hash);
    h.addData(data);
    std::string hashResult = h.result(Hash::Type::Binary);

    // 创建上下文
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(publicKey_, NULL);
    assert(ctx != NULL);
    // 初始化上下文
    int ret = EVP_PKEY_verify_init(ctx);
    assert(ret == 1);
    // 设置填充方式
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);
    // 设置签名算法的哈希函数
    ret = EVP_PKEY_CTX_set_signature_md(ctx, hashFunction_.at(hash)());
    assert(ret == 1);

    // 校验
    ret = EVP_PKEY_verify(ctx, reinterpret_cast<const unsigned char *>(sign.data()), sign.size(),
                          reinterpret_cast<const unsigned char *>(hashResult.data()), hashResult.size());
    EVP_PKEY_CTX_free(ctx);
    return ret == 1;
}

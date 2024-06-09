#include "aescrypto.h"
#include "base64.h"
#include <QCryptographicHash>

AesCrypto::AesCrypto(Algorithm algorithm, QByteArray key, QObject *parent)
    : QObject{parent}
{
    switch (algorithm) {
    case aes_128_ecb:
    case aes_128_cbc:
    case aes_128_cfb:
    case aes_128_ofb:
    case aes_128_ctr:
        assert(key.size() == 16);
        break;
    case aes_192_ecb:
    case aes_192_cbc:
    case aes_192_cfb:
    case aes_192_ofb:
    case aes_192_ctr:
        assert(key.size() == 24);
        break;
    case aes_256_ecb:
    case aes_256_cbc:
    case aes_256_cfb:
    case aes_256_ofb:
    case aes_256_ctr:
        assert(key.size() == 32);
        break;
    }

    type_ = algorithm;
    key_ = key;
}

QByteArray AesCrypto::encrypt(QByteArray text)
{
    return aescrypto(text, ENCTYPT);
}

QByteArray AesCrypto::decrypt(QByteArray text)
{
    return aescrypto(text, DECTRPT);
}

QByteArray AesCrypto::aescrypto(QByteArray text, CryptoType type)
{
    // 1. 生成对称加密算法上下文对象
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    assert(ctx);

    // 2. 初始化上下文对象
    unsigned char iv[AES_BLOCK_SIZE];
    generateIvec(iv);
    int ret = EVP_CipherInit_ex(ctx, algorithms_[type_](),
                                NULL, reinterpret_cast<unsigned char*>(key_.data()),
                                iv, type);
    assert(ret);

    // 3. 加密 or 解密
    if (type == CryptoType::DECTRPT) {
        Base64 base64;
        text = base64.decode(text);
    }
    unsigned char out[text.size() + AES_BLOCK_SIZE];
    int outLen = 0;  int totalLen = 0;
    ret = EVP_CipherUpdate(ctx, out, &outLen,
                           reinterpret_cast<unsigned char*>(text.data()), text.size());
    totalLen += outLen;
    assert(ret);

    ret = EVP_CipherFinal_ex(ctx, out + outLen, &outLen);
    totalLen += outLen;
    assert(ret);

    EVP_CIPHER_CTX_free(ctx);
    if (type == CryptoType::ENCTYPT) {
        Base64 base64;
        return base64.encode(reinterpret_cast<char *>(out), totalLen);
    }
    return QByteArray(reinterpret_cast<char*>(out), totalLen);
}

void AesCrypto::generateIvec(unsigned char *ivec)
{
    QCryptographicHash hash(QCryptographicHash::Md5);   // md5生成16 bytes的hash值
    hash.addData(key_);                                 // 添加密钥
    std::string res = hash.result().toStdString();                     // 根据密钥生成hash值，并当做初试向量
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        ivec[i] = res.data()[i];
    }
}

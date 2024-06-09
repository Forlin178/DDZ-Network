#ifndef AESCRYPTO_H
#define AESCRYPTO_H

#include <QObject>
#include <QByteArray>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <QMap>

class AesCrypto : public QObject
{
    Q_OBJECT
public:
    using algorithmFunc = const EVP_CIPHER *(*)(void);

    enum Algorithm {
        // 16 bytes
        aes_128_ecb,
        aes_128_cbc,
        aes_128_cfb,
        aes_128_ofb,
        aes_128_ctr,
        // 24 bytes
        aes_192_ecb,
        aes_192_cbc,
        aes_192_cfb,
        aes_192_ofb,
        aes_192_ctr,
        // 32 bytes
        aes_256_ecb,
        aes_256_cbc,
        aes_256_cfb,
        aes_256_ofb,
        aes_256_ctr,
    };

    enum CryptoType {DECTRPT, ENCTYPT};

    explicit AesCrypto(Algorithm algorithm, QByteArray key, QObject *parent = nullptr);

    QByteArray encrypt(QByteArray text);        // 加密
    QByteArray decrypt(QByteArray text);        // 解密

private:
    QByteArray aescrypto(QByteArray text, CryptoType type);     // enc=1加密  enc=0解密
    void generateIvec(unsigned char *ivec);     // 通过哈希根据密钥生成初始向量
signals:

private:
    Algorithm type_;
    QByteArray key_;    // 密钥
    const QMap<Algorithm, algorithmFunc> algorithms_{
        {aes_128_ecb, EVP_aes_128_ecb},
        {aes_128_cbc, EVP_aes_128_cbc},
        {aes_128_cfb, EVP_aes_128_cfb128},
        {aes_128_ofb, EVP_aes_128_ofb},
        {aes_128_ctr, EVP_aes_128_ctr},

        {aes_192_ecb, EVP_aes_192_ecb},
        {aes_192_cbc, EVP_aes_192_cbc},
        {aes_192_cfb, EVP_aes_192_cfb128},
        {aes_192_ofb, EVP_aes_192_ofb},
        {aes_192_ctr, EVP_aes_192_ctr},

        {aes_256_ecb, EVP_aes_256_ecb},
        {aes_256_cbc, EVP_aes_256_cbc},
        {aes_256_cfb, EVP_aes_256_cfb128},
        {aes_256_ofb, EVP_aes_256_ofb},
        {aes_256_ctr, EVP_aes_256_ctr}
    };
};

#endif // AESCRYPTO_H

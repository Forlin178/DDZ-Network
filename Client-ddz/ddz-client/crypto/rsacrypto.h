#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <QObject>
#include <QByteArray>
#include <QCryptographicHash>
#include <openssl/evp.h>
#include <QMap>

class RsaCrypto : public QObject
{
    Q_OBJECT
public:
    using hashFunc = const EVP_MD *(*)(void);

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

    explicit RsaCrypto(QObject *parent = nullptr);
    // 从密钥文件 加载到 publicKey_/privateKey_
    explicit RsaCrypto(QByteArray filename, KeyType type, QObject *parent = nullptr);
    ~RsaCrypto();

    // 从 data(内存) 加载到 publicKey_/privateKey_
    void parseStringToKey(QByteArray data, KeyType type);


    // 生成非对称加密的密钥对，两个文件分别存储公钥和密钥
    void generateRsaKey(KeyLength keyLength, QByteArray pub = "public.pem", QByteArray pri = "private.pem");
    // 通过 公钥 对 对称加密的密钥 进行加密
    QByteArray pubKeyEncrypt(QByteArray data);
    // 通过 私钥 对 对称加密的密钥 进行解密
    QByteArray priKeyDecrypt(QByteArray data);
    // 数据签名: 通过 私钥 对 数据的哈希值 进行加密
    QByteArray sign(QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);
    // 签名校验: 通过 公钥 对 签证 进行解密，并对数据求哈希值与签证里的哈希对比
    bool varify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);

signals:

private:
    EVP_PKEY *publicKey_{ NULL };
    EVP_PKEY *privateKey_{ NULL };
    const QMap<QCryptographicHash::Algorithm, hashFunc> hashFunction_ {
        {QCryptographicHash::Md4, EVP_md4},
        {QCryptographicHash::Md5, EVP_md5},
        {QCryptographicHash::Sha1, EVP_sha1},
        {QCryptographicHash::Sha224, EVP_sha224},
        {QCryptographicHash::Sha256, EVP_sha256},
        {QCryptographicHash::Sha384, EVP_sha384},
        {QCryptographicHash::Sha512, EVP_sha512},
        {QCryptographicHash::Sha3_224, EVP_sha3_224},
        {QCryptographicHash::Sha3_256, EVP_sha3_256},
        {QCryptographicHash::Sha3_384, EVP_sha3_384},
        {QCryptographicHash::Sha3_512, EVP_sha3_512}
    };
};

#endif // RSACRYPTO_H

//
// Created by 38491 on 2024/4/7.
//

#include "Base64.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>


std::string Base64::encode(const std::string &data) {
    return encode(data.data(), data.size());
}

std::string Base64::encode(const char *data, int length) {
    BIO *bio64 = BIO_new(BIO_f_base64());
    BIO *bioMem = BIO_new(BIO_s_mem());

    // 组织bio链  base64 -> mem
    BIO_push(bio64, bioMem);

    // 编码 数据流向 data -> base64 -> mem
    BIO_write(bio64, data, length);
    BIO_flush(bio64);

    // 把编码后的数据读出来
    BUF_MEM *buf;
    BIO_get_mem_ptr(bioMem, &buf);
    std::string str(buf->data, buf->length);

    // 释放bio链
    BIO_free_all(bio64);
    return str;
}

std::string Base64::decode(const std::string &base64) {
    return decode(base64.data(), base64.size());
}

std::string Base64::decode(const char *base64, int length) {
    BIO *bio64 = BIO_new(BIO_f_base64());
    BIO *bioMem = BIO_new(BIO_s_mem());

    // 组织bio链  base64 -> mem
    BIO_push(bio64, bioMem);

    // 将待解码的数据提前写入到 mem
    BIO_write(bioMem, base64, length);

    // 解码 数据流向 buf <- bio64 <- mem <- base64
    // 解码后 原文长度 < base64->length
    char *buf = new char[length];
    int bufLength = BIO_read(bio64, buf, length);
    std::string str(buf, bufLength);

    BIO_free_all(bio64);
    delete[]buf;
    return str;
}

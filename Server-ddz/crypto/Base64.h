//
// Created by 38491 on 2024/4/7.
//

#ifndef SERVER_DDZ_BASE64_H
#define SERVER_DDZ_BASE64_H

#include <string>

class Base64 {
public:
    explicit Base64() = default;

    // Base64 编码
    static std::string encode(const std::string &data);

    static std::string encode(const char *data, int length);

    // Base64 解码
    static std::string decode(const std::string &base64);

    static std::string decode(const char *base64, int length);
};

#endif //SERVER_DDZ_BASE64_H

#pragma once

#include <string>

class Buffer
{
public:
	Buffer(int size);
	~Buffer();

	inline char* getData() { return data_; }
	inline int getCapacity() { return capacity_; }
	inline int getReadPos() { return readPos_; }
	inline int getWritePos() { return writePos_; }
	inline void setReadPos(int pos) { readPos_ = pos; }

	// 扩容
	void extendRoom(int size);

	// 写内存
	int appendString(const char* data, int size);
	int appendString(const char* data);
    int appendString(const std::string& string);
    int appendHead(int length);
    int appendPackage(const std::string& data);     // 数据包: 数据头(4B) + 数据块
	int socketRead(int fd);

	// 返回\r\n子字符串的开头位置指针
	char* findCRLF();

    // 读取指定字节数的数据
    std::string readData(int length);

	// 发送数据
	int sendData(int socket);
private:
	char* data_;
	int capacity_;
	int readPos_{ 0 };
	int writePos_{ 0 };
};


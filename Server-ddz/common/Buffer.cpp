#include "Buffer.h"
#include "Log.h"
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

Buffer::Buffer(int size) {
    data_ = (char *) malloc(size * sizeof(char));
    capacity_ = size;
    memset(data_, 0, size * sizeof(char));
}

Buffer::~Buffer() {
    // 考虑到后续可能需要用realloc扩容
    // 所以data申请内存不用new 而是malloc，free与其配套
    if (data_ != nullptr) {
        free(data_);
    }
}

void Buffer::extendRoom(int size) {
    // 1.内存够用   --不扩容
    if (capacity_ - writePos_ >= size) {
        return;
    }
    // 2.内存需合并 --不扩容
    if (capacity_ - writePos_ + readPos_ >= size) {
        memmove(data_, data_ + readPos_, writePos_ - readPos_);
        writePos_ = writePos_ - readPos_;
        readPos_ = 0;
        return;
    }
    // 3.合并也不够 --扩容
    data_ = (char *) realloc(data_, capacity_ + size);
    memset(data_ + capacity_, 0, size);
    capacity_ += size;
}

int Buffer::appendString(const char *data, int size) {
    if (data == nullptr || size <= 0) return -1;

    extendRoom(size);
    memcpy(data_ + writePos_, data, size);
    writePos_ += size;
    return 0;
}

int Buffer::appendString(const char *data) {
    return appendString(data, strlen(data));
}

int Buffer::socketRead(int fd) {
    int writeable = capacity_ - writePos_;
    struct iovec vec[2];
    vec[0].iov_base = data_ + writePos_;
    vec[0].iov_len = writeable;
    char *temp = (char *) malloc(40960);
    vec[1].iov_base = temp;
    vec[1].iov_len = 40960;

    int result = readv(fd, vec, 2);
    if (result == -1) return -1;
    else if (result <= writeable) {
        // 数据只用buf->data就装下了
        writePos_ += result;
    } else {
        writePos_ = capacity_;
        appendString(temp, result - writeable);
    }
    free(temp);
    return result;
}

char *Buffer::findCRLF() {
    char *ptr = (char *) memmem(data_ + readPos_, writePos_ - readPos_, "\r\n", 2);
    return ptr;
}

int Buffer::sendData(int socket) {
    // readable: 可读数据大小
    // count: 实际发送数据的大小，受fd缓冲区影响
    int readable = writePos_ - readPos_;
    if (readable > 0) {        //发送数据
        int count = send(socket, data_ + readPos_, readable, MSG_NOSIGNAL);
        if (count > 0) {
            readPos_ += count;
            usleep(1);
        }
        return count;
    }
    return 0;
}

int Buffer::appendString(const std::string &string) {
    int ret = appendString(string.data(), string.size());
    return ret;
}

int Buffer::appendHead(int length) {
    int len = htonl(length);
    // int 转化为 char* 后可能中间有\0，注意不能用strlen
    std::string head(reinterpret_cast<char *>(&len), sizeof(int));
    appendString(head);
    return 0;
}

int Buffer::appendPackage(const std::string &data) {
    appendHead(data.size());
    appendString(data);
    return 0;
}

std::string Buffer::readData(int length) {
    std::string msg(data_ + readPos_, length);
    readPos_ += length;
    return msg;
}

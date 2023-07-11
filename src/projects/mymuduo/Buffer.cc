#include "Buffer.h"

#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

ssize_t Buffer::readFd(int fd, int* saveErrno) {
    char extrabuf[65536] = {0}; // 栈上的内存空间 64K

    struct iovec vec[2];

    const size_t writeable = writableBytes();  // Buffer底层缓冲区剩余的可写空间
    // 用vec来临时存储fd中的数据
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    // 如果底层buffer够用，就不用extrabuf，否则就使用extrabuf
    const int iovcnt = (writeable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0) {
        *saveErrno = errno;
    } else if (n < writeable) { // 只写入buffer底层
        writerIndex_ += n;
    } else {    // 写入extrabuf
        writerIndex_ = buffer_.size();
        // 此时底层buffer已写满，从extrabuf开始写，写入n - writeable的数据
        append(extrabuf, n - writeable);
    }

    return n;
}

// 从fd上发送数据
ssize_t Buffer::writeFd(int fd, int* saveErrno) {
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0) {
        *saveErrno = errno;
    }
    return n;
}
#pragma once

#include <vector>
#include <string>
#include <algorithm>

// 网络库底层的缓冲器类型定义

class Buffer {
    public:
        static const size_t kCheapPrepend = 8;
        static const size_t kInitialSize = 1024;

        explicit Buffer(size_t initialSize = kInitialSize)
            : buffer_(kCheapPrepend + initialSize),
              readerIndex_(kCheapPrepend),
              writerIndex_(kCheapPrepend)
        {}

        size_t readableBytes() const {
            return writerIndex_ - readerIndex_;
        }

        size_t writableBytes() const {
            return buffer_.size() - writerIndex_;
        }

        size_t prependableBytes() const {
            return readerIndex_;
        }

        // 返回缓冲区中可读数据的起始地址
        const char* peek() const {
            return begin() + readerIndex_;
        }

        // 复位len长度的数据
        void retrieve(size_t len) {
            if (len < readableBytes()) {
                // 应用只读取了read缓冲区数据的一部分(len),还剩下readerIndex_+len -> writerIndex_这部分
                readerIndex_ += len;    
            } else {
                retrieveAll();
            }
        }

        void retrieveAll() {
            readerIndex_ = writerIndex_ = kCheapPrepend;
        }

        // 把onMessage函数上报的Buffer数据转成string类型数据返回
        std::string retrieveAllAsString() {
            return retrieveAsString(readableBytes());
        }

        std::string retrieveAsString(size_t len) {
            std::string result(peek(), len);
            retrieve(len);  // 把已经读取了数据的缓冲区进行复位操作
            return result;
        }

        void ensureWriteableBytes(size_t len) {
            if (writableBytes() < len) {
                makeSpace(len); // 扩容函数
            }
        }
        
        // 把[data, data+len]内存上的数据，添加到writable缓冲区中
        void append(const char* data, size_t len) {
            ensureWriteableBytes(len);
            std::copy(data, data+len, beginWrite());
            writerIndex_ += len;
        }

        char* beginWrite() {
            return begin() + writerIndex_;
        }

        const char* beginWrite() const {
            return begin() + writerIndex_;
        }

        // 从fd上读取数据
        ssize_t readFd(int fd, int* saveErrno);
        // 从fd上发送数据
        ssize_t writeFd(int fd, int* saveErrno);
    private:
        char* begin() {
            // vector底层数组首元素的地址，也就是数组的起始地址
            return &*buffer_.begin();   
        }
        const char* begin() const {
            // vector底层数组首元素的地址，也就是数组的起始地址
            return &*buffer_.begin();   
        }

        void makeSpace(size_t len) {
            /**
             * | kCheapPrepend | reader  |  writer  |
             * | kCheapPrepend |          len           |
             * 当前整个buffer_中的空闲区域，依旧小于所需要的空间len
             * */ 
            if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
                buffer_.resize(writerIndex_ + len);
            } else {    // 将数据移到readerIndex_起始
                size_t readable = readableBytes();
                std::copy(begin() + readerIndex_,
                            begin() + writerIndex_,
                            begin() + kCheapPrepend);
                readerIndex_ = kCheapPrepend;
                writerIndex_ = readerIndex_ + readable;
            }
        }
        std::vector<char> buffer_;
        size_t readerIndex_;
        size_t writerIndex_;
};
#include "Poller.h"
#include "EPollPoller.h"

#include <stdlib.h>

// EventLoop可以通过该接口获取默认的IO复用的具体实现
Poller* Poller::newDafaultPoller(EventLoop* loop) {
    if (::getenv("MUDUO_USE_POLL")) {
        return nullptr; // 生成poll实例
    } else {
        return new EPollPoller(loop); // 生成epoll实例
    }
}
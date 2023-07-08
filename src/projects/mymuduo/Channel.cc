#include "Channel.h"
#include "logger.h"

#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd) 
    : loop_(loop), 
      fd_(fd), 
      events_(0),
      revents_(0),
      index_(-1),
      tied_(false) {}

Channel::~Channel() {
}

// channel的tie方法什么时候调用过？
void Channel::tie(const std::shared_ptr<void>& obj) {
    tie_ = obj;
    tied_ = true;
}

/**
 * 当改变channel所表示fd的events事件后，update负责在poller里更改fd相应的事件 epoll_ctl
 * EventLoop => Channel + Poller
*/
void Channel::update() {
    // 通过channel所属的EventLoop，调用poller的相应方法，注册fd的events事件
    loop_->updateChannel(this);
}

// 在channel所属的EventLoop中，把当前的channel删除
void Channel::remove() {
    loop_->removeChannel(this);
}

// fd得到poller通知以后，处理事件。
void Channel::handleEvent(Timestamp receiveTime) {
    if (tied_) {
        // 提升智能指针级别
        std::shared_ptr<void> guard = tie_.lock();
        if (guard) {
            handleEventWithGuard(receiveTime);
        }
    }
    else {
        handleEventWithGuard(receiveTime);
    }
}

// 根据poller通知的channel发生的具体事件，由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp recevieTime) {
    LOG_INFO("channel handleEvent revents:%d\n", revents_);

    // 关闭处理
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
        if (closeCallback_) {
            closeCallback_();
        }
    }
    // 错误处理
    if (revents_ & EPOLLERR) {
        if (errorCallback_) {
            errorCallback_();
        }
    }
    // 读处理
    if (revents_ && !(EPOLLPRI & EPOLLIN)) {
        if (readCallback_) {
            readCallback_(recevieTime);
        }
    }
    // 写处理
    if (revents_ && EPOLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}
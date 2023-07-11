#pragma once

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"

#include <functional>

class EventLoop;

class Acceptor : noncopyable {
    public:
        using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;

        Acceptor(EventLoop* loop, const InetAddress &listenAddr, bool reuseport);
        ~Acceptor();

        void setNewConnectionCallback(const NewConnectionCallback& cb) {
            NewConnectionCallback_ = std::move(cb);
        }

        bool listenning() const { return listenning_; }
        void listen();
    private:
        void handleRead();

        EventLoop* loop_;   // Acceptor用的就是用户定义的那个baseloop，也成为mainloop
        Socket acceptSocket_;
        Channel acceptChannel_;
        NewConnectionCallback NewConnectionCallback_;
        bool listenning_;
};
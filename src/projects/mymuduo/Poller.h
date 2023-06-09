#pragma once

#include <vector>
#include <unordered_map>

#include "noncopyable.h"
#include "Timestamp.h"

class Channel;
class EventLoop;

// muduo库中多路事件分发器的核心 IO复用模块
class Poller : noncopyable {
    public:
        using ChannelList = std::vector<Channel*>;

        Poller(EventLoop* loop);
        virtual ~Poller() = default;

        // 给所有IO复用保留统一的接口
        virtual Timestamp poll(int timeoutMs, ChannelList *activateChannels) = 0;
        virtual void updateChannel(Channel* channel) = 0;
        virtual void removeChannel(Channel* channel) = 0;

        // 判断参数channel是否在当前Poller中
        bool hasChannel(Channel* channel) const;

        // EventLoop可以通过该接口获取默认的IO复用的具体实现
        static Poller* newDafaultPoller(EventLoop* loop);
    protected:
        // map的key: sockfd  value: sockfd所属的channel通道类型
        using ChannelMap = std::unordered_map<int, Channel*>;
        ChannelMap channels_;
    private:
        EventLoop* ownerLoop_;        
};
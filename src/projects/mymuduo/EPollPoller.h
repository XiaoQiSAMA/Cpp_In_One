#pragma once

#include "Poller.h"
#include "Timestamp.h"

#include <vector>
#include <sys/epoll.h>

class Channel;

/**
 * epoll的使用
 * epoll_create: 构造函数
 * epoll_ctl(add/mod/del): UpdateChannel()/removeChannel()
 * epoll_wait: poll()
*/

class EPollPoller : public Poller {
    public:
        EPollPoller(EventLoop* loop);
        ~EPollPoller();

        // 重写Poller中的接口
        Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
        void updateChannel(Channel* channel) override;
        void removeChannel(Channel* channel) override;
    private:
        static const int KInitEventListSize = 16;

        // 填写活跃的连接
        void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
        // 更新channel通道
        void update(int operation, Channel* channel);

        using EventList = std::vector<epoll_event>;

        int epollfd_;
        EventList events_;
};
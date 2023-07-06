#include "EPollPoller.h"
#include "logger.h"
#include "Channel.h"

#include <errno.h>
#include <strings.h>

// channel的状态，对应channel中的index_ = -1
// channel未添加到Poller中
const int kNew = -1;
// channel已添加到Poller中
const int kAdded = 1;
// channel从Poller中删除
const int kDeleted = 2;


EPollPoller::EPollPoller(EventLoop* loop)
    : Poller(loop),  
      epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(KInitEventListSize) 
{
    if (epollfd_ < 0) {
        LOG_FATAL("epoll_create error:%d \n", errno);
    }
}

EPollPoller::~EPollPoller() {
    ::close(epollfd_);
}

Timestamp EPollPoller::poll(int timeoutMs, ChannelList *activeChannels) {
    // LOG_DEBUG输出日志，否则影响epoll运行效率
    LOG_INFO("func=%s => fd total count:%d\n", __FUNCTION__, channels_.size());
    
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno;
    Timestamp now(Timestamp::now());

    if (numEvents > 0) {
        LOG_INFO("%d events happened \n", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    } else if (numEvents == 0) {
        LOG_DEBUG("%s timeout! \n", __FUNCTION__);
    } else {
        if (saveErrno != EINTR) {
            errno = saveErrno;
            LOG_ERROR("EPollPoller::poll() err!");
        }
    }
    return now;
}


void EPollPoller::updateChannel(Channel* channel) {
    int index = channel->index();
    LOG_INFO("func=%s => fd=%d events=%d index=%d \n", __FUNCTION__, channel->fd(), channel->events(), index);

    // channel未在Poller上注册
    if (index == kNew || index = kDeleted) {
        if (index == kNew) {
            int fd = channel->fd();
            channels_[fd] = channel;
        }

        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    } else {    // channel已经在Poller上注册了
        int fd = channel->fd();
        if (channel->isNoneEvent()) {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDeleted);
        } else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}


void EPollPoller::removeChannel(Channel* channel) {
    int fd = channel->fd();
    // 从Poller中的ChannelMap中移除
    channels_.erase(fd);
    
    LOG_INFO("func=%s => fd=%d \n", __FUNCTION__, fd);
    // channel更新状态
    int index = channel->index();
    if (index == kAdded) {
        // 如果已经添加进epoll了，先进行删除
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}

// 填写活跃的连接
void EPollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const {
    for (int i = 0; i < numEvents; ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        // EventLoop拿到了它的poller给它返回的所有发生事件的channel列表
        activeChannels->push_back(channel);
    }
}

// 更新channel通道
void EPollPoller::update(int operation, Channel* channel) {
    epoll_event event;
    bzero(&event, sizeof event);

    int fd = channel->fd();

    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;

    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
            LOG_ERROR("epoll_ctl del error:%d\n", errno);
        } else {
            LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
        }
    }
}
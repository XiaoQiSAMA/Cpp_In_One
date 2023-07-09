#pragma once

#include "noncopyable.h"

#include <functional>
#include <vector>
#include <string>
#include <memory>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
    public:
        using ThreadInitCallback = std::function<void(EventLoop*)>;

        EventLoopThreadPool(EventLoop* baseloop, const std::string &nameArg);
        ~EventLoopThreadPool();

        void setThreadNum(int numThreads) { numThreads_ = numThreads; }

        void start(const ThreadInitCallback &cb = ThreadInitCallback());

        // 如果工作在多线程中，baseLoop_默认以轮询的方式分配channel给subLoop
        EventLoop* getNextLoop();

        std::vector<EventLoop*> getAllLoops();

        bool started() const { return started_; }
        const std::string name() const { return name_; }
    private:
        EventLoop* baseLoop_;   // 单线程下的base loop
        std::string name_;
        bool started_;
        int numThreads_;
        int next_;
        std::vector<std::unique_ptr<EventLoopThread>> threads_;
        std::vector<EventLoop*> loops_;
};
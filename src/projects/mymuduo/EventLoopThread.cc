#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb,
    const std::string &name) 
    : loop_(nullptr),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc, this), name),
      mutex_(),
      cond_(),
      callback_(cb)
{
}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != nullptr) {
        loop_->quit();
        thread_.join();
    }
}

/**
 * one loop per thread
 * startLoop()开始新线程
 * 新线程绑定了threadFunc(),并创建一个新loop
 * 新loop开始进行poll
 * startLoop()返回该loop的地址
*/
EventLoop* EventLoopThread::startLoop() {
    thread_.start();    // 启动底层的新线程

    EventLoop* loop = nullptr;
    {   
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop_ == nullptr) {
            cond_.wait(lock);
        }
        // 等待cond_的notify，表示loop创建完成
        loop = loop_;
    }
    return loop;
}

// 下面这个方法，是在单独的新线程里运行的
void EventLoopThread::threadFunc() {
    EventLoop loop;     // 创建一个独立eventloop，和上面的线程是一一对应的，one loop per thread

    if (callback_) {
        callback_(&loop);
    }

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop(); // EventLoop loop => Poller.poll 开始IO操作

    std::unique_lock<std::mutex> lock(mutex_);
    loop_ = nullptr;
}
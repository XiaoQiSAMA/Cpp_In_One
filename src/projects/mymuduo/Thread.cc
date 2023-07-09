#include "Thread.h"
#include "CurrentThread.h"

#include <semaphore.h>

// 静态变量要单独在类外初始化
std::atomic_int Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false),
      joined_(false),
      tid_(0),
      func_(std::move(func)),
      name_(name)
{
    setDefaultName();
}
Thread::~Thread() {
    if (started_ && !joined_) {
        thread_->detach();  // 设置分离线程。这样线程就会跟随主线程退出来一起退出
    }
}

void Thread::start() {
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);

    // 开启线程
    thread_ = std::shared_ptr<std::thread>(new std::thread([&](){
        // 获取线程的tid
        tid_ = CurrentThread::tid();
        sem_post(&sem);
        // 开启一个新线程，专门执行该线程函数
        func_();
    }));

    // 通过sem通信，直到sem_post返回sem的信号后才能继续执行，否则无法获得tid值
    sem_wait(&sem);
}
void Thread::join() {
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()) {
        char buf[32] = {0};
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}

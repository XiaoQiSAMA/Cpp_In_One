# C++语言级别多线程编程

* thread/mutex/condition_variable
* lock_quard/unique_lock
* atomic 原子类型 基于CAS操作的原子类型 线程安全的
* sleep_for

[code_for_threads](../../src/thread_pool/threads.cpp)

* t.join(): 等待t线程结束，当前线程继续往下执行
* t.detach(): 把t线程设置分离线程，主线程结束整个进程结束，所有子线程自动结束。

## mutex互斥锁

[code_for_mutex](../../src/thread_pool/mutex.cpp)

竞态条件: 多线程程序执行的结果是一致的，不会随着CPU对线程不同的调用顺序而产生不同的运行结果

## 线程同步通信

生产者-消费者线程模型

[code_for_tele](../../src/thread_pool/threads_tele.cpp)

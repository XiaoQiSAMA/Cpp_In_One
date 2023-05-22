#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>    // C++ STL的所有容器都不是线程安全的
using namespace std;

std::mutex mtx;
std::condition_variable cv; // 定义条件变量，做线程间的同步通信操作

// 线程安全的queue
// 生产者生产一个物品，通知消费者消费一个；消费完了，消费者再通知生产者继续生产物品
class Queue{
    public:
        void put(int val){
            // lock_guard<std::mutex> guard(mtx);
            unique_lock<std::mutex> lck(mtx);
            while (!que.empty()){
                // que不为空，生产者应该通知消费者取消费，消费完了，再继续生产
                // 生产者线程进入等待状态，并且把mtx互斥锁释放掉
                
                cv.wait(lck);

            }
            
            que.push(val);
            // 其他线程从等待状态 => 阻塞状态 => 获取互斥锁才能继续执行
            cv.notify_all();    // 通知其他所有线程，我生产了物品
            cout << "Producer put: " << val << " goods" << endl;
        }
        int get(){
            // lock_guard<std::mutex> guard(mtx);
            unique_lock<std::mutex> lck(mtx);
            while (que.empty()){
                // 消费者线程发现que为空，通知生产者线程去生产
                
                cv.wait(lck);
            }
            int val = que.front();
            que.pop();
            cv.notify_all();    // 通知生产者进行生产
            cout << "Consumer get: " << val << " goods" << endl;
            return val;
        }
    private:
        queue<int> que;
};

// 生产者线程
void producer(Queue *que){
    for (int i = 0; i <= 10; ++i){
        que->put(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// 消费者线程
void comsumer(Queue *que){
    for (int i = 0; i <= 10; ++i){
        que->get();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
    Queue que;
    std::thread t1(producer, &que);
    std::thread t2(comsumer, &que);

    t1.join();
    t2.join();

    return 0;
}
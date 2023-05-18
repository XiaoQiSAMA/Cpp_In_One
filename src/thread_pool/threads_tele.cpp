#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>    // C++ STL的所有容器都不是线程安全的
using namespace std;

std::mutex mtx;

// 线程安全的queue
// 
class Queue{
    public:
        void put(int val){
            lock_guard<std::mutex> guard(mtx);
            que.push(val);
            cout << "Producer put: " << val << " goods" << endl;
        }
        int get(){
            lock_guard<std::mutex> guard(mtx);
            int val = que.front();
            que.pop();
            cout << "Consumer get: " << val << " goods" << endl;
            return val;
        }
    private:
        queue<int> que;
};



int main(){

    return 0;
}
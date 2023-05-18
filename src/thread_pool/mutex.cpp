#include <iostream>
#include <thread>
#include <list>
#include <mutex>
using namespace std;

/*
thread 模拟车站三个窗口卖票程序
*/
int count = 100;    // 车站有100张票，由三个窗口一起卖票
// 全局互斥锁
std::mutex mtx;

void sellTicket(int index){
    
    while (count > 0){  
        // count = 1的时候进入循环，会导致多个线程共同使得count--
        // mtx.lock();
        // 双重判断
        {   
            // 在局部作用域中起作用，出了作用域则自动析构做unlock
            lock_guard<std::mutex> lock(mtx);   // scoped_ptr
            // unique_lock<std::mutex> ulock(mtx);  // unique_ptr
            // ulock.lock();
            if (count > 0){
                // 临界区代码段要保持原子操作，让互斥锁起作用
                cout << "Sell: " << index << " has sell : " << count << " tickets!" << endl;
                count--;  
            }
            // ulock.unlock();
        }
        // 代码段执行return，则锁无法释放导致死锁
        // mtx.unlock(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
    }    
    
}


int main(){
    list<std::thread> tlist;
    for (int i = 1; i <= 3; ++i){
        tlist.push_back(std::thread(sellTicket, i));
    }

    for (std::thread &t : tlist){
        t.join();
    }

    cout << "All threads completed!" << endl;
    return 0;
}















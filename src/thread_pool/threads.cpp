#include <iostream>
#include <thread>

void threadHandler1(int time){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "hello thread1!" << std::endl;
}

int main(){
    // 创建了一个线程对象，传入一个线程函数
    std::thread t1(threadHandler1, 2);
    // // 主线程等待子线程结束，主线程继续往下运行
    // t1.join();

    // 把子线程设置为分离线程，直接不管子线程
    t1.detach();
    
    std::cout << "main thread done!" << std::endl;

    /*
    主线程关闭，而子线程还未完成则会导致异常
    */
    return 0;
}
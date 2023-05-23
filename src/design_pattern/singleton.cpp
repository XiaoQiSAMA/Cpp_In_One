#include <iostream>
// #include <thread>
#include <mutex>
using namespace std;

std::mutex mtx;

# if 0
// 饿汉式单例：一定是线程安全
class Singleton{
    public:
        static Singleton* getInstance(){    // #3 获取类的唯一实例对象的接口方法
            return &instance;
        }
    private:
        static Singleton instance;  // #2 定义一个唯一的类的实例对象
        Singleton(){    // #1 构造函数私有化
            // 初始化内容过多，会影响性能
        }
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
};
Singleton Singleton::instance;  // 创建静态实例
# endif

# if 0
// 懒汉式单例 ==> 线程安全(锁 + 双重判断)
class Singleton{
    public:
        static Singleton* getInstance(){    // #3 获取类的唯一实例对象的接口方法
            // 多个线程在进入if判断时线程不安全
            // lock_guard<std::mutex> guard(mtx);  // 力度太大了
            if (instance == nullptr){
                lock_guard<std::mutex> guard(mtx);
                if (instance == nullptr){
                    instance = new Singleton();
                }
            }
            return instance;
        }
    private:
        static Singleton *volatile instance;  // #2 定义的类的实例对象指针
        Singleton(){    // #1 构造函数私有化
            
        }
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
};
Singleton*volatile Singleton::instance = nullptr;  // 创建静态实例
# endif

// 懒汉式单例线程安全
class Singleton{
    public:
        static Singleton* getInstance(){    // #3 获取类的唯一实例对象的接口方法
            // 函数静态局部变量的初始化，在汇编指令上已经自动添加线程互斥指令了
            static Singleton instance;
            return &instance;
        }
    private:
        Singleton(){    // #1 构造函数私有化
            
        }
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
};
// Singleton Singleton::instance;  // 创建静态实例

int main(){
    // 共享一个对象
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
    Singleton *p3 = Singleton::getInstance();


    return 0;
}
#include <iostream>
#include <memory>
#include <thread>
using namespace std;

template <typename T>
class CSmartPtr{
    public:
        CSmartPtr(T *ptr = nullptr): mptr(ptr) {}
        ~CSmartPtr() {  delete mptr;}
        CSmartPtr(const CSmartPtr<T> &src){
            // 各自开辟自己内存空间
            mptr = new T(*src.mptr);
        }

        T& operator*() {   return *mptr;}
        T* operator->() {   return mptr;}
    private:
        T *mptr;
};

/*
muduo库中
多线程访问共享对象的线程安全问题
*/
class A{
    public:
        A() {   cout << "A()" << endl;}
        ~A() {  cout << "~A()" << endl;}
        void testA(){   cout << "funcA()" << endl;}
};
// 子线程
// void handler01(A *q){
//     std::this_thread::sleep_for(std::chrono::seconds(2));
//     // q在访问A对象的时候，需要侦测A对象是否存活
//     q->testA();
// }
// 智能指针子线程
void handler01(weak_ptr<A> pw){
    shared_ptr<A> sp = pw.lock();
    if (sp != nullptr){
        sp ->testA();
    } else{
        cout << "Object A has been ~A()" << endl;
    }
    
}
// main线程
int main(){
    // A *p = new A();

    // thread t1(handler01, p);

    // // std::this_thread::sleep_for(std::chrono::seconds(2));

    // delete p;

    // t1.join();

    // 智能指针来解决
    {
        shared_ptr<A> p(new A());

        thread t1(handler01, weak_ptr<A>(p));

        // std::this_thread::sleep_for(std::chrono::seconds(2));

        t1.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}



#if 0
int main(){
    /*
    不带引用计数的智能指针
    autp_ptr: c++库
    c++11:
    scoped_ptr
    unique_ptr

    想要让两个指针都指向同一块内存，但是由于浅拷贝无法做到
    */

   // 不重写复制构造函数会导致p2指向p1的空间，在析构时会析构两次，导致崩溃
   // 重写后又因浅拷贝无法指向同一内存
//    CSmartPtr<int> p1(new int);
//    CSmartPtr<int> p2(p1);

    /* 无法实现
    auto_ptr在ptr2复制构造时，会将ptr1的资源抢走，ptr1的指针置空

    不推荐使用auto_ptr
    vector<auto_ptr<int>> vec1; vec2(vec1);
    vec1的指针全置为nullptr
    */
    // auto_ptr<int> ptr1(new int);
    // auto_ptr<int> ptr2(ptr1);

    // *ptr2 = 20;
    // cout << *ptr1 << endl;

    /* 推荐使用unique_ptr
    unique_ptr无法直接左值复制构造
    unique_ptr(const unique_ptr<T> &) = delete;
    unique_ptr<T> & operator=(const unique_ptr<T> &) = delete;

    unique_ptr使用的是右值引用
    unique_ptr(unique_ptr<T> &&src);
    unique_ptr<T> & operator=(unique_ptr<T> &&src);

    */
    unique_ptr<int> ptr1(new int);
    unique_ptr<int> ptr2(std::move(ptr1));

    // p2持有资源
    *ptr2 = 20;
    cout << *ptr1 << endl;

   return 0;
}

#endif
#include <iostream>
#include <memory>
using namespace std;

/*
shared_ptr: 强智能指针，可以改变资源的引用计数
weak_ptr:   弱智能指针，不会改变资源的引用计数

weak_ptr => shared_ptr => 资源(内存)

weak_ptr只是一个观察者，无法去直接使用指向资源的func等

强智能指针循环引用(交叉引用)是什么问题？导致什么结果？怎么解决？
解决：定义对象的时候用强智能指针，引用对象的地方使用弱智能指针
*/

class B;
class A{
    public:
        A() {   cout << "A()" << endl;}
        ~A() {  cout << "~A()" << endl;}
        void testA(){   cout << "funcA()" << endl;}
        weak_ptr<B> _ptrb;
        // shared_ptr<B> _ptrb;
};

class B{
    public:
        B() {   cout << "B()" << endl;}
        ~B() {  cout << "~B()" << endl;}
        void func(){
            // 把弱智能指针升为强智能指针
            shared_ptr<A> ps = _ptra.lock();
            // 根据资源是否能够使用来判断
            if (ps != nullptr){
                ps->testA();
            }
        }
        weak_ptr<A> _ptra;
        // shared_ptr<A> _ptra;
};

int main(){
    shared_ptr<A> pa(new A());
    shared_ptr<B> pb(new B());

    // 交叉引用问题：
    // 造成对象new出来的资源无法释放，资源泄露问题
    pa->_ptrb = pb;
    pb->_ptra = pa;

    cout << pa.use_count() << endl;
    cout << pb.use_count() << endl;
    
    pb->func();
    
    return 0;
}
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

void hello1(){
    cout << "hello world!" << endl;
}

void hello2(string str){
    cout << str << endl;
}

int sum(int a, int b){
    return a + b;
}

void hello(string str){
    cout << str << " out of class " << endl;
}
class Test{
    public:
        // 调用函数必须依赖一个对象 void (Test::*pfunc)(string)
        void hello(string str) {  cout << str << endl;}
};

/* -------------------------------------- */
template <typename Fty>
class myfunction {};

// 特列化
/*
template <typename RT, typename AT>
class myfunction<RT(AT)>{
    public:
        using PFUNC = RT(*)(AT);
        myfunction(PFUNC pfunc) : _pfunc(pfunc) {}
        RT operator() (AT arg){
            return _pfunc(arg);
        }

    private:
        PFUNC _pfunc;
};

template <typename RT, typename AT1, typename AT2>
class myfunction<RT(AT1, AT2)>{
    public:
        using PFUNC = RT(*)(AT1, AT2);
        myfunction(PFUNC pfunc) : _pfunc(pfunc) {}
        RT operator() (AT1 arg1, AT2 args2){
            return _pfunc(arg1, args2);
        }

    private:
        PFUNC _pfunc;
};
*/

// 参数模板化
template <typename RT, typename... AT>
class myfunction<RT(AT...)>{
    public:
        // 指针类型 RT(*)(AT...)
        using PFUNC = RT(*)(AT...);
        myfunction(PFUNC pfunc) : _pfunc(pfunc) {}
        RT operator() (AT... arg){
            return _pfunc(arg...);
        }

    private:
        // 函数指针对象去指向函数
        PFUNC _pfunc;
};

# if 1
int main(){
    myfunction<void(string)> func1(hello);
    func1("hello world");   // func1.operator()(args)
    
    myfunction<int(int, int)> func2(sum);
    cout << func2(10, 20) << endl;

    return 0;
}
#endif

# if 0
int main(){
    // 从function的类模板定义处，看到希望用一个函数类型实例化function
    function<void()> func1 = hello1;
    func1();    // func1.operator()() ==> hello1();
    function<void(string)> func2 = hello2;
    func2("hello world2!");

    function<int(int, int)> func3 = sum;
    cout << func3(10, 20) << endl;

    // operator()
    function<int(int, int)> func4 = [](int a, int b)->int {return a + b;};
    cout << func4(30, 40) << endl;

    function<void(Test*, string)> func5 = &Test::hello;
    // 不能传右值，只能传左值
    Test test;
    func5(&test, "call Test::hello()");

    return 0;

}
#endif
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

int main(){
    // 从function的类模板定义处，看到希望用一个函数类型实例化function
    function<void()> func1 = hello1;
    func1();    // func1.operator()() ==> hello1();

    
}
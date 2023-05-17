# bind & function

## bind

* bind1st: operator()的第一个形参变量绑定成一个确定的值
* bind2st: operator()的第二个形参变量绑定成一个确定的值

$\bf{作用:}$ 绑定器 + 二元函数对象 ==> 一元函数对象

[binder code](../../src/cpp_upgrade/binder%26function.cpp)

## function

绑定器，函数对象，lambda表达式 它们只能在一条语句中

[function object code](../../src/cpp_upgrade/function_object.cpp)

function原理

## lambda

函数对象的缺点：

1. 使用在泛型算法参数传递
2. 比较性质/自定义操作
3. 优先级队列
4. 智能指针

语法表达: \[捕获外部变量\](形参列表)->返回值{操作代码}

如果lambda表达式的返回值不需要，那么"->返回值"可以省略

$\bf{分析:}$

* []: 表示不捕获任何外部变量
* \[=\]: 以传值的方式捕获外部所有变量
* \[&\]: 以传引用的方式捕获外部的所有变量
* \[this\]: 捕获外部的this指针
* \[-, &a\]: 以传值的方式捕获外部的所有变量，但是a变量以传引用的方式捕获
* \[a, b\]: 以传值的方式捕获外部变量a,b

<!-- $\bf{mutable int a }$ 设置变量mutable，类外可以进行修改 -->

```c++
template <typename T=void>
class TestLambda01{
    void operator()(){
        cout << "hello world!" << endl;
    }
};

int main(){
    // auto func1 = []()->void {cout << "hello world!" << endl; };
    auto func1 = [](){cout << "hello world!" << endl; };
    func1();
    

    // 函数对象的缺点
    TestLambda01<> t1;
    t1();

    return 0;
}
```

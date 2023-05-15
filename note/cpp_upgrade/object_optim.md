# 对象优化

c++编译器对于对象构造的优化：用临时对象生成新对象的时候，临时对象就不产生了，直接构造新对象

code: src/cpp_upgrade/object_optim.cpp:line33

## 对象构造过程

```c++
Test t1(10, 10);  // 1. Test(int, int)
int main(){
    Test t2(20, 20);    // 3. Test(int, int)
    Test t3 = t2;       // 4. Test(const Test&)
    // static对象知道程序结束再析构
    static Test t4 = Test(30, 30);  // 5. Test(int, int)
    t2 = Test(40, 40);  // 6. Test(int, int) operator=  ~Test()
    // (50, 50)逗号表达式，只取其最后一个数做单变量输入：t2=Test(50,)
    t2 = (Test)(50, 50);    // 7. Test(int, int)  operator=  ~Test()
    t2 = 60;    // 8. Test(int, int)  operator=  ~Test()
    Test *p1 = new Test(70, 70);    // 9. Test(int, int)  
    Test *p2 = new Test[2];     // 10. Test(int, int)  Test(int, int)  
    Test *p3 = &test(80, 80);   // 11. Test(int, int)  ~Test()
    const Test &p4 = test(90, 90)   // 12. Test(int, int)  
    delete p1;  // 13. ~Test()
    delete []p2;    // 14. ~Test() ~Test()
}
Test t5(100, 100); // 2. Test(int, int)
```

## 对象的优化

```c++
// 不能返回局部的或者临时对象的指针或引用
Test GetObject(Test t){
    // 形参t已经初始化成新的对象，在函数结束时析构
    int val = t.getData();
    Test tmp(val);
    return tmp;
}

// // 错误示范：临时对象tmp会在语句结束后就析构，而指针则成为空指针
// Test* GetObject(Test t){
//     int val = t.getData();
//     Test tmp(val);
//     // static Test tmp(val);    此时tmp为全局对象，不会在语句结束时释放
//     return &tmp;
// }

int main(){
    Test t1;
    Test t2;
    // 实参 ==> 形参: Test(const Test&)
    // t1调用拷贝构造来初始化GetObject中的t
    t2 = GetObject(t1);
    return 0;
}
```

## 对象优化规则

1. 函数参数传递过程中，对象优先按引用传递，不要按值传递

    ```c++
    Test* GetObject(Test &t){
        int val = t.getData();
        Test tmp(val);
        return tmp;
    }
    ```

2. 函数返回对象的时候应该优先返回一个临时对象，而不是返回一个定义过的对象

    ```c++
    // 同理Test t = Test(val),此时不会发生新建对象再复制构造这一过程，而是只进行构造
    Test* GetObject(Test &t){
        int val = t.getData();
        return Test(val);
    }
    ```

3. 接收返回值是对象的函数调用的时候，优先按初始化的方式接收，不要按赋值的方式接收

    ```c++
    int main(){
        Test t1;
        // 减少一次t2的构造函数与析构
        Test t2 = GetObject(t1);
        return 0;
    }
    ```

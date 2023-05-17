# C++11 内容

## 关键字和语法

* auto: 根据右值来推导类型
* nullptr: 给指针专用(能够和整数进行区别)
* foreach: 可以遍历数组，容器等

    ```c++
    for (Type val : container){
        cout << val << " ";
    }
    ```

* 右值引用: move移动语义函数和forward类型完美转发函数
* 模板新特性: typename... A  表示可变参(类型参数)
  
## 绑定器和函数对象

* function: 函数对象
* bind: 绑定器  bind1st和bind2nd + 二元函数对象 => 一元函数对象
* lambda表达式
  
## 智能指针

* shared_ptr 和 weak_ptr

## 容器

* set 和 map : 红黑树 O(logn)
* unordered_set 和 unordered_map : 哈希表 O(1)
* array: 数组
* forward_list: 前向链表

## C++多线程编程

* 跨平台C++语言级别的多线程
* Linux: createThread
* windows: pthread_create, clone

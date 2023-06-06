# SGI STL

内存池、进程池、线程池、连接池

## 内存池

allocator 空间配置器

```c++
template<typename T, typename _Alloc=allocator<T>>
class vecotr{
}
```

allocator:

* allocate: 负责给容器开辟内存空间  ==> malloc
* deallocate: 负责释放容器内存空间  ==> free
* construct:  负责给容器构造对象    ==> 定位new实现
* destory:  负责析构容器的对象      ==> p->~T()

1. 分离了对象的内存开辟，对象构造
2. 分离了对象的析构，内存的释放

一级allocate 内存管理: malloc/free

二级allocate 内存管理: 内存池的实现

[Analysis_for_SourceCode](SGI_STL_SourceCode.pdf)

优点:

1. 对于每一个字节数的chunk块分配，都是给出一部分进行使用，另一部分作为备用，这个备用可以给当前字节数使用，也可以给其他字节数使用。
2. 对于备用内存池划分完chunk块以后，如果还有剩余的很小的内存块，再次分配的时候会把这些小的内存块再次分配出去，备用内存池使用的干干净净！
3. 当指定字节数内存分配失败以后，有一个异常处理的过程，8-128字节所有的chunk块进行查看，如果哪个字节数有空闲的chunk块，直接借一个出去。
4. 如果以上操作失败，会调用oom_malloc一个预先设置好的malloc内存分配后的回调函数。
   * 回调函数未设置: malloc throw bad alloc
   * 回调函数设置:   (* oom_malloc_handler)(); 无限循环调用

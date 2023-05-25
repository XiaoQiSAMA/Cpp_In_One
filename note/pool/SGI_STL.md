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

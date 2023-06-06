# Nginx内存池

## nginx内存分配

1. nginx小块内存分配(指针偏移)
   * 没有提供任何的内存释放函数，实际上从小块内存的分配方式来看(直接通过last指针偏移来分配内存)，它也没法进行小块内存的回收
2. nginx大块内存分配(小块内存中分配大块内存的头信息)
   * 内存释放ngx_free函数，直接释放头信息中alloc指向的内存

[PDF_For_Structure](nginx_SourceCode.pdf)

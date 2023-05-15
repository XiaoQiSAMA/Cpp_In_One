# CMyString的问题

$\bf CMyString中的效率问题$

```c++
CMyString GetString(CMyString &str){
    const char *pstr = str.c_str();
    CMyString tmpStr(pstr);     // 构造函数
    return tmpStr;  // 复制构造
}


int main(){
    CMyString str1("aaaaaaaaaaaaaaaaaaaaaaa");  // 构造函数
    CMyString str2;     // 构造函数
    str2 = GetString(str1);     // 赋值构造
    cout << str2.c_str() <<endl;

    return 0;
}
```

一次复制构造多占用一次内存，赋值操作也多占用一次内存

$\bf CMyString中的右值引用$

```c++
CMyString operator+(const CMyString &lhs, const CMyString &rhs){
    // // 开辟一个新temp空间来存储数据
    // // 此处占了一部分内存，后续再复制构造开辟CMyString的新内存，浪费
    // char *ptmp = new char[strlen(lhs.mptr) + strlen(rhs.mptr) + 1];
    // strcpy(ptmp, lhs.mptr);
    // strcat(ptmp, rhs.mptr);
    // // 返回一个新的临时CMyString
    // CMyString tmpStr(ptmp);
    // delete []ptmp;  // 产出new出来的内存
    // return tmpStr;

    // 直接构建CMyString来接受数据，不需开辟额外内存
    CMyString tmpStr;
    tmpStr.mptr = new char[strlen(lhs.mptr) + strlen(rhs.mptr) + 1];
    strcpy(tmpStr.mptr, lhs.mptr);
    strcat(tmpStr.mptr, rhs.mptr);
    // 返回一个新的合成CMyString
    return tmpStr;
}


int main(){
    CMyString str1 = "hello";
    CMyString str2 = "world!";
    // 复制构造: 析构tmp对象时只需要删除空指针
    CMyString str3 = str1 + str2;
    return 0;
}

```

直接右值引用将tmpStr的指针指向新的数据内存处，内存进行后续的直接拷贝。复制构造时只需删除tmpStr指向这部分数据的空指针，改为复制构造对象的新指针。

$\bf Vector中的复制构造$

```c++
// 左值引用的复制构造
CMyString(const CMyString &str){
    mptr = new char[strlen(str.mptr) + 1];
    strcpy(mptr, str.mptr);
}
// 右值引用的复制构造：直接夺取tmp的内存
CMyString(CMyString &&str){
    mtpr = str.mptr;
    str.mptr = nullptr;
}

int main(){
    CMyString str1 = "aaa";

    vector<CMyString> vec;
    vec.reserve(10);

    vec.push_back(str1);
    vec.push_back(CMyString("bbb"));
    return 0;
}
```

在面对左值时，是左值引用的复制构造；面对右值时，是右值引用的复制构造。

但是，右值引用本身还是左值，vector无法识别，需要std::move(object)来强转对象为右值引用

模板方法来解决

```c++
// 引用折叠
// CMyString& + && = CMyString&
// CMyString&& + && = CMyString&&
template <typename Ty>  // 函数模板的类型推演+引用折叠
void push_back(Ty &&val){
    if (full())
        expand();
    // // val仍是左值
    // _allocator.construct(_last, val);

    // move: 移动语义，强转为右值类型 (int&&) a
    // forward: 类型的完美转发,能识别val是左值还是右值
    _allocator.construct(_last, std::forward<Ty>(val));
    _last++;
}

```

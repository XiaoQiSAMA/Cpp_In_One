#include<iostream>
using namespace std;

class Test{
    public:
        Test(int i = 0): ma(i) {
            cout << "Test(int)" << endl;
        };
        ~Test() {
            cout << "~Test()" << endl;
        };
        Test(const Test &t){
            cout << "Test (const Test &)" << endl;
        };
        Test &operator=(const Test &t){
            cout << "operator=" << endl;
            ma = t.ma;
            return *this;
        };

    private:
        int ma;
};

int main(){
    Test t1;
    Test t2(t1);
    Test t3 = t1;
    // Test(20) 显式生成临时对象 生存周期：所在语句
    /*
    c++编译器对于对象构造的优化：用临时对象生成新对象的时候，临时对象就不产生了，直接构造新对象
    */
    Test t4 = Test(20);

    t4 = t2;
    // 此时该临时对象非用于直接生成新对象，需重新构造
    t4 = Test(30);

    // 显式生成
    t4 = (Test)30;  // int -> Test: 当Test的构造函数是Test(int),可以强转int为Test类型
    
    // 隐式生成
    t4 = 30;    // Test(30) int -> Test(int)

    cout << "----------------------" << endl;
    // p指向的是一个已经析构的临时对象
    // 新版本c++已经不能这么写了
    Test *p = &Test(40);

    const Test &ref = Test(50);
    cout << "----------------------" << endl;
    return 0;
}
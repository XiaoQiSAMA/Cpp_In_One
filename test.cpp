#include<iostream>
using namespace std;

class A{
    public:
        A(int i = 0): a(i){};
        int a;
};


int main(){
    A *a1 = new A(1);
    // int *p1 = 0;
    // cout << "&p1: " << &p1 << endl;
    // // cout << "*p1: " << *p1 << endl;
    // cout << "p1: " << p1 << endl;
    // cout << "&A: " << &A() << endl;
    cout << "&a1: " << &a1->a << endl;
    // cout << "*p1: " << *p1 << endl;
    cout << "a1: " << a1->a << endl;
    return 0;
}
#include <iostream>
using namespace std;

class Car {
    public:
        virtual void show() = 0;
};

class BMW : public Car {
    void show() {
        cout << "This is a BMW" << endl;
    }
};

class Audi : public Car {
    void show() {
        cout << "This is a Audi" << endl;
    }
};

class Benz : public Car {
    void show() {
        cout << "This is a Benz" << endl;
    }
};

// // 装饰器基类
// class CarDecorator : public Car {
//     public:
//         CarDecorator(Car *p): pCar(p) {}
//     private:
//         Car *pCar;
// };

// 装饰器1
class Decorator01 : public Car {
    public:
        Decorator01(Car *p): pCar(p) {}
        void show() {
            pCar->show();
            cout << "Add function 1" << endl;
        }
    private:
        Car *pCar;
};

// 装饰器2
class Decorator02 : public Car {
    public:
        Decorator02(Car *p): pCar(p) {}
        void show() {
            pCar->show();
            cout << "Add function 2" << endl;
        }
    private:
        Car *pCar;
};

int main(){
    Car *p1 = new Decorator01(new BMW());
    p1 = new Decorator02(p1);
    p1->show();

    Car *p2 = new Decorator02(new Audi());
    p2->show();

    Car *p3 = new Decorator01(new Benz());
    p3->show();

    return 0;
}
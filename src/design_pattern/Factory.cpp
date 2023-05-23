#include <iostream>
#include <memory>
using namespace std;

enum CarType{
    BMW, AUDI
};
class Car{
    public:
        Car(string name): _name(name) {}
        virtual void show() = 0;
    protected:
        string _name;
};

class Bmw : public Car {
    public:
        Bmw(string name): Car(name) {}
        void show(){
            cout << "Get a BMW: " << _name << endl;
        }
};

class Audi : public Car{
    public:
        Audi(string name): Car(name) {}
        void show(){
            cout << "Get a Audi: " << _name << endl;
        }
};

// 工厂方法
class Factory{
    public:
        virtual Car* createCar(string name) = 0;
};
// 宝马工厂
class BMWFactory : public Factory{
    public:
        Car* createCar(string name){
            return new Bmw(name);
        }
};

class AudiFactory : public Factory{
    public:
        Car* createCar(string name){
            return new Audi(name);
        }
};

int main(){
    unique_ptr<Factory> BMWfactory(new BMWFactory());
    unique_ptr<Factory> Audifactory(new AudiFactory());
    unique_ptr<Car> p1(BMWfactory->createCar("X6"));
    unique_ptr<Car> p2(Audifactory->createCar("A8"));
    p1->show();
    p2->show();

    return 0;
}

# if 0
// 简单工厂
class SimpleFactory{
    public:
        Car* CreateCar(CarType ct){ // 不符合软件设计："开-闭"
            switch (ct)
            {
            case BMW:
                return new Bmw("X1");
            case AUDI:
                return new Audi("A6");
            default:
                cerr << "Parameter is error!" << ct << endl;
                break;
            }       
            return nullptr;
        }
};

int main(){

    // SimpleFactory *factory = new SimpleFactory();
    // Car *p1 = factory->CreateCar(BMW);
    // Car *p2 = factory->CreateCar(AUDI);
    // p1->show();
    // p2->show();

    // delete p1;
    // delete p2;
    // delete factory;

    // 智能指针
    unique_ptr<SimpleFactory> factory(new SimpleFactory());
    unique_ptr<Car> p1(factory->CreateCar(BMW));
    unique_ptr<Car> p2(factory->CreateCar(AUDI));

    p1->show();
    p2->show();

    return 0;
}
# endif
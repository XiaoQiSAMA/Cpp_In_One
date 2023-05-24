#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
using namespace std;

// 数据对象发生改变，相应的依赖对象应及时收到通知
class Observer {
    public:
        // 处理消息的接口
        virtual void handle(int msgid) = 0;
};

// 第一个观察者实例
class Observer1 : public Observer {
    public:
        void handle(int msgid) {
            switch (msgid)
            {
            case 1:
                cout << "Observer1 recv 1 msg!" << endl;
                break;
            
            case 2:
                cout << "Observer1 recv 2 msg!" << endl;
                break;

            default:
                cout << "Observer1 recv unknow msg!" << endl;
                break;
            }
        }
};

// 第二个观察者实例
class Observer2 : public Observer {
    public:
        void handle(int msgid) {
            switch (msgid)
            {
            case 2:
                cout << "Observer2 recv 2 msg!" << endl;
                break;

            default:
                cout << "Observer2 recv unknow msg!" << endl;
                break;
            }
        }
};

// 第三个观察者实例
class Observer3 : public Observer {
    public:
        void handle(int msgid) {
            switch (msgid)
            {
            case 1:
                cout << "Observer3 recv 1 msg!" << endl;
                break;
            
            case 3:
                cout << "Observer3 recv 3 msg!" << endl;
                break;

            default:
                cout << "Observer3 recv unknow msg!" << endl;
                break;
            }
        }
};

// 主数据类
class Subject {
    public:
        // 添加新的observer与msgid响应事件绑定
        void addObserber(Observer *obser, int msgid) {
            // unordered_map不会产生重复msgid的obser
            _subMap[msgid].push_back(obser);
            // auto it = _subMap.find(msgid);
            // if (it != _subMap.end()){
            //     it->second.push_back(obser);
            // } else {
            //     list<Observer*> lis;
            //     lis.push_back(obser);
            //     _subMap.insert({msgid, lis});
            // }
        }
        // 主数据类发生改变，通知相应的观察者对象处理事件
        void dispatch(int msgid){
            auto it = _subMap.find(msgid);
            if (it != _subMap.end()){
                for (Observer *pObser : it->second){
                    pObser->handle(msgid);
                }
            }
        }
    private:
        unordered_map<int, list<Observer*>> _subMap; 
};

int main() {
    Subject subject;
    Observer *p1 = new Observer1();
    Observer *p2 = new Observer2();
    Observer *p3 = new Observer3();

    subject.addObserber(p1, 1);
    subject.addObserber(p1, 2);
    subject.addObserber(p2, 2);
    subject.addObserber(p3, 1);
    subject.addObserber(p3, 3);
    
    int msgid = 0;
    for (;;) {
        cout << "Input: ";
        cin >> msgid;
        // if (msgid == -1);
        //     break;
        subject.dispatch(msgid);
    }
    return 0;
}
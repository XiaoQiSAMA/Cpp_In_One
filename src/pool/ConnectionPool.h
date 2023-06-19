/*
实现连接池功能模块
*/
#ifndef _CONNECTIONPOOL_H_
#define _CONNECTIONPOOL_H_

#include <string>
#include <queue>
#include <mutex>
#include "Connection.h"
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
using namespace std;

class ConnectionPool {
    public:
        // 获取连接池对象实例
        static ConnectionPool* getConnectionPool();
        // 给外部提供接口，从连接池中获取一个可用的空闲连接
        shared_ptr<Connection> getConnection();
    private:
        ConnectionPool();   // 单例#1 构造函数私有化
        bool LoadConfig();  // 从配置文件中加载配置项
        void produceConnectionTask();   // 运行在独立的线程中，专门负责生产新连接
        void scannerConnectionTask();   // 扫描超过maxIdleTime时间的空闲连接，进行对应的连接回收

        string _ip;     // mysql的ip地址
        unsigned short _port;       // mysql的端口号 3306
        string _username;       // mysql登录用户名
        string _dbname;         // 数据库名称
        string _password;       // mysql登陆密码
        int _initSize;      // 连接池的初始连接量
        int _maxSize;       // 连接池的最大连接量
        int _maxIdleTime;       // 连接池最大空闲时间
        int _connectionTimeout;     // 连接池获取连接的超时时间

        queue<Connection*> _connectionQue;      // 存储mysql连接的队列
        mutex _queueMutex;          // 维护连接队列的线程安全互斥锁
        atomic_int _connectionCnt;      // 记录连接所创建的connection连接的总数量，提供线程安全的++操作
        condition_variable cv;      // 设置条件变量，用于连接生产线程和连接消费线程间的通信
};

// 线程安全的懒汉单例接口
ConnectionPool* ConnectionPool::getConnectionPool() {
    static ConnectionPool pool; // lock 和 unlock
    cout << "ConnectionPool create successful!" << endl;
    return &pool;
}

// 加载配置项
bool ConnectionPool::LoadConfig() {
    FILE *pf = fopen("C:\\Users\\hp-pc\\OneDrive\\work\\more_c++\\src\\pool\\mysql.ini", "r");
    if (pf == nullptr) {
        LOG("mysql.ini file is not exist!");
        return false;
    }
    // LOG("mysql.ini file finded!");

    while (!feof(pf)) {
        char line[1024] = { 0 };
        fgets(line, 1024, pf);
        string str = line;
        int idx = str.find('=', 0);
        if (idx == -1) {
            // LOG("idx == -1");
            continue;
        }

        // password=123456\n
        int endidx = str.find('\n', idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endidx - idx - 1);

        if (key == "ip") {
            _ip = value;
        }
        else if (key == "port") {
            _port = atoi(value.c_str());
        }
        else if (key == "username") {
            _username = value;
        }
        else if (key == "dbname") {
            _dbname = value;
        }
        else if (key == "password") {
            _password = value;
        }
        else if (key == "initSize") {
            _initSize = atoi(value.c_str());
        }
        else if (key == "maxSize") {
            _maxSize = atoi(value.c_str());
        }
        else if (key == "maxIdleTime") {
            _maxIdleTime = atoi(value.c_str());
        }
        else if (key == "connectionTimeout") {
            _connectionTimeout = atoi(value.c_str());
        }
    }
    return true;
} 

// 连接池的构造
ConnectionPool::ConnectionPool() {
    // 加载配置项
    if (!LoadConfig()) {
        // cout << "Load mysql.ini failed! " << endl;
        return;
    }

    // cout << "_initSize: " << _initSize << endl;

    // 创建初始数量的连接
    for (int i = 0; i < _initSize; ++i) {
        Connection *p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        p->refreshAliveTime();  // 刷新空闲连接存活时间
        _connectionQue.push(p);
        _connectionCnt++;
    }
    // cout << "Init ConnectionPool size: " << _connectionQue.size() << endl;

    // 启动一个新的线程，作为连接的生产者
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();

    // 启动一个新的定时线程，扫描超过_maxIdleTimed的空闲连接，进行对于理解的回收
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}

 // 运行在独立的线程中，专门负责生产新连接
void ConnectionPool::produceConnectionTask() {
    for (;;) {
        unique_lock<mutex> lock(_queueMutex);
        while (!_connectionQue.empty()) {
            cv.wait(lock);      // 队列不为空，生产线程进入等待
        }
        
        // 连接数量没有达到上限，继续创建新的连接
        if (_connectionCnt < _maxSize) {
            Connection *p = new Connection();
            p->connect(_ip, _port, _username, _password, _dbname);
            p->refreshAliveTime();  // 刷新空闲连接存活时间
            _connectionQue.push(p);
            _connectionCnt++;
        }
        
        // 通知消费者线程，可以消费连接了
        cv.notify_all();
    }
}

shared_ptr<Connection> ConnectionPool::getConnection() {
    unique_lock<mutex> lock(_queueMutex);
    while (_connectionQue.empty()) {
        // sleep
        if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout))) {
            // cout << "connectQue size: " << _connectionQue.size() << endl;
            if (_connectionQue.empty()) {
                LOG("TimeOut for getting connection...connection failed!");
                return nullptr;
            }
        }
    }

    /*
    shared_ptr智能指针析构时，会把connection资源直接delete，相当于调用connection
    的析构函数，connection就被close了，这里需要自定义shared_ptr的释放资源的方式，
    把connection直接归还到queue当中
    */
    shared_ptr<Connection> sp(_connectionQue.front(), 
        [&](Connection *pcon) {
            // 服务器线程调用的，要考虑队列的线程安全操作
            unique_lock<mutex> lock(_queueMutex);
            pcon->refreshAliveTime();  // 刷新空闲连接存活时间
            _connectionQue.push(pcon);
        });
    
    _connectionQue.pop();
    // 消费连接完了之后，通知生产者线程进行检查连接队列
    cv.notify_all();

    return sp;


}

void ConnectionPool::scannerConnectionTask() {
    for (;;) {
        // sleep模拟定时效果
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));

        // 扫描整个队列，释放多余的连接
        unique_lock<mutex> lock(_queueMutex);
        while (_connectionCnt > _initSize) {
            Connection* p = _connectionQue.front();
            if (p->getAliveTime() > (_maxIdleTime * 1000)) {
                _connectionQue.pop();
                _connectionCnt--;
                delete p;   // 调用~Connection()来释放连接
            }
            else {
                break;  // 队头的理解没有超过_maxIdleTime则后续的连接都不会超过
            }
        }
    }
}

#endif
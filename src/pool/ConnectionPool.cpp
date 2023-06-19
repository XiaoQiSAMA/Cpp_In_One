#include "ConnectionPool.h"
#include "Connection.h"
using namespace std;

// 线程安全的懒汉单例接口
ConnectionPool* ConnectionPool::getConnectionPool() {
    static ConnectionPool pool; // lock 和 unlock
    return &pool;
}

// 加载配置项
bool ConnectionPool::LoadConfig() {
    FILE *pf = fopen("mysql.ini", "r");
    if (pf == nullptr) {
        LOG("mysql.ini file is not exist!");
        return false;
    }

    while (!feof(pf)) {
        char line[1024] = { 0 };
        fgets(line, 1024, pf);
        string str = line;
        int idx = str.find('=', 0);
        if (idx == -1) {
            return false;
        }

        // password=123456\n
        int endidx = str.find('\n', idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endidx - idx - 1);

        if (key == "ip") {
            _ip = value;
        }
        if (key == "port") {
            _port = atoi(value.c_str());
        }
        if (key == "username") {
            _username = value;
        }
        if (key == "dbname") {
            _dbname = value;
        }
        if (key == "password") {
            _password = value;
        }
        if (key == "initSize") {
            _initSize = atoi(value.c_str());
        }
        if (key == "maxSize") {
            _maxSize = atoi(value.c_str());
        }
        if (key == "maxIdleTime") {
            _maxIdleTime = atoi(value.c_str());
        }
        if (key == "connectionTimeout") {
            _connectionTimeout = atoi(value.c_str());
        }
    }
    return true;
} 

// 连接池的构造
ConnectionPool::ConnectionPool() {
    // 加载配置项
    if (!LoadConfig()) {
        return;
    }

    // 创建初始数量的连接
    for (int i = 0; i < _initSize; ++i) {
        Connection *p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        _connectionQue.push(p);
        _connectionCnt++;
    }

    // 启动一个新的线程，作为连接的生产者
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
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
            _connectionQue.push(p);
            _connectionCnt++;
        }

        // 通知消费者线程，可以消费连接了
        cv.notify_all();
    }
}

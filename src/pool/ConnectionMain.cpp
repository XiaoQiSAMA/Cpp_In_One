#include <iostream>
// #include "Connection.h"
#include "ConnectionPool.h"
using namespace std;

int main() {

    clock_t begin = clock();
    

    // 多线程
    thread t1([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i) {
            // 连接池
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
                "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t2([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i) {
            // 连接池
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
                "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t3([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i) {
            // 连接池
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
                "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t4([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; ++i) {
            // 连接池
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
                "zhang san", 20, "male");
            sp->update(sql);
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Cost Time: " << clock() - begin << endl;

    #if 0
    for (int i = 0; i < 1000; ++i) {
        /* 单线程
        Connection conn;
        char sql[1024] = {0};
        sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
            "zhang san", 20, "male");
    
        conn.connect("127.0.0.1", 3306, "root", "0701", "chat");
        conn.update(sql);
        */

        // 连接池
        shared_ptr<Connection> sp = cp->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
            "zhang san", 20, "male");
        sp->update(sql);
        
    }
    cout << "Cost Time: " << clock() - begin << endl;
    #endif

    return 0;
}
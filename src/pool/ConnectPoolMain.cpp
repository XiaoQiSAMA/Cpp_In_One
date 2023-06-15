#include <iostream>
#include "ConnectPool.h"
using namespace std;

int main() {
    ConnectPool conn;
    char sql[1024] = {0};
    sprintf(sql, "insert into users(name, age, sex) values('%s', '%d', '%s')",
        "zhang san", 20, "male");
    
    conn.connect("127.0.0.1", 3306, "root", "0701", "chat");
    conn.update(sql);

    return 0;
}
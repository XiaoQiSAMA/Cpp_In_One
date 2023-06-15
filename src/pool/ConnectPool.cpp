// #include "public.h"
// #include "mysql.h"
#include "ConnectPool.h"
// #include <iostream>
using namespace std;

// 初始化数据库连接
ConnectPool::ConnectPool()
{
    _conn = mysql_init(nullptr);
}
// 释放数据库连接资源
ConnectPool::~ConnectPool()
{
    if (_conn != nullptr)
    mysql_close(_conn);
}
// 连接数据库
bool ConnectPool::connect(string ip, unsigned short port, string user, string password,
string dbname)
{
    MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(),
    password.c_str(), dbname.c_str(), port, nullptr, 0);
    return p != nullptr;
}
// 更新操作 insert、delete、update
bool ConnectPool::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG("更新失败:" + sql);
        return false;
    }
    return true;
}
// 查询操作 select
MYSQL_RES* ConnectPool::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG("查询失败:" + sql);
        return nullptr;
    }
    return mysql_use_result(_conn);
}
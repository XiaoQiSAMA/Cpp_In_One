#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main() {
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("xiaoqi");
    req.set_pwd("123456");

    // 对象数据序列化
    std::string send_str;
    if (req.SerializeToString(&send_str)) {
        std::cout << send_str << std::endl;
    }

    // 反序列化
    LoginRequest req_;
    if (req_.ParseFromString(send_str)) {
        std::cout << req_.name() << std::endl;
        std::cout << req_.pwd() << std::endl;
    }
    
    return 0;
}
#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main() {
    // // 封装了login请求对象的数据
    // LoginRequest req;
    // req.set_name("xiaoqi");
    // req.set_pwd("123456");

    // // 对象数据序列化
    // std::string send_str;
    // if (req.SerializeToString(&send_str)) {
    //     std::cout << send_str << std::endl;
    // }

    // // 反序列化
    // LoginRequest req_;
    // if (req_.ParseFromString(send_str)) {
    //     std::cout << req_.name() << std::endl;
    //     std::cout << req_.pwd() << std::endl;
    // }

    // 对象类型设置
    // LoginResponse rsp;
    // ResultCode* rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("Lunched failed!");

    GetFriendListsResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);

    // 列表类型设置
    User* user1 = rsp.add_friend_list();
    user1->set_name("xiaoqi");
    user1->set_age(24);
    user1->set_sex(User::MAN);

    User* user2 = rsp.add_friend_list();
    user2->set_name("xiaoqi2");
    user2->set_age(24);
    user2->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;

    User getuser1 = rsp.friend_list(0);
    std::cout << getuser1.name() << std::endl;
    std::cout << getuser1.age() << std::endl;
    return 0;
}
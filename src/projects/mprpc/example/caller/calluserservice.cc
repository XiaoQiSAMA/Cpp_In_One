#include <iostream>

#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char **argv) {
    // 整个程序启动后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("xiaoqi");
    request.set_pwd("990701");

    // rpc方法的响应
    fixbug::LoginResponse response;

    // RpcChannel->RpcChannel::CallMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    stub.Login(nullptr, &request, &response, nullptr);

    // 一次rpc调用完成，读调用的结果
    if (response.result().errcode() == 0) {
        std::cout << "rpc login response success : " << response.sucess() << std::endl;
    } else {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }
    
    // 演示调用远程发布的rpc方法Register
    fixbug::RegisterRequest req;
    req.set_id(1111);
    req.set_name("qiqi");
    req.set_pwd("000000");

    fixbug::RegisterResponse rsp;

    stub.Register(nullptr, &req, &rsp, nullptr);
    // 一次rpc调用完成，读调用的结果
    if (rsp.result().errcode() == 0) {
        std::cout << "rpc Register response success : " << rsp.sucess() << std::endl;
    } else {
        std::cout << "rpc Register response error : " << rsp.result().errmsg() << std::endl;
    }

    return 0;
}
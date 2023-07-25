#include <iostream>
#include <string>
#include <vector>

#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendService : public fixbug::FriendServiceRpc {
    public:
        std::vector<std::string> GetFriendsList(uint32_t userid) {
            std::cout << "do GetFriendList service! userid : " << userid << std::endl; 
            std::vector<std::string> vec;
            vec.push_back("sb");
            vec.push_back("fuck");
            vec.push_back("bitch");
            return vec;
        }

        // rpc方法
        void GetFriendsList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done) {
            uint32_t userid = request->userid();
            std::vector<std::string> friendList = GetFriendsList(userid);

            response->mutable_result()->set_errcode(0);
            response->mutable_result()->set_errmsg("");
            for (std::string &name : friendList) {
                std::string *p = response->add_friends();
                *p = name;
            }

            done->Run();
            
        }
};

int main(int argc, char **argv) {
    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象。把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务发布节点  Run()之后进程进入阻塞状态，等待远超的rpc调用请求
    provider.Run();

    return 0;
}
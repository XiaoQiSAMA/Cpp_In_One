#pragma once
#include "google/protobuf/service.h"

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <unordered_map>

// 框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider {
    public:
        // 这里是框架提供给外部使用的，可以发布rpc方法的函数接口
        void NotifyService(google::protobuf::Service *service);

        // 启动rpc服务节点，开始提供rpc远超网络调用服务
        void Run();
    
    private:
        // 组合了EventLoop
        muduo::net::EventLoop m_eventLoop;

        // 服务类型信息
        struct ServiceInfo
        {
            google::protobuf::Service* m_service;   // 保持服务对象
            std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // 保持服务方法
        };
        // 存储注册成功的服务对象和其服务方法的所以信息
        std::unordered_map<std::string, ServiceInfo> m_serviceMap;

        // 新的socket连接回调
        void OnConnection(const muduo::net::TcpConnectionPtr&);
        // 已连接用户的读写事件回调
        void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
        // Closure的回调操作，用于序列化rpc的响应和网络发送
        void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
};

template <typename Class, typename Arg1, typename Arg2>
class MethodClosure : public google::protobuf::Closure {
public:
    typedef void (Class::*MethodType)(Arg1, Arg2);

    MethodClosure(Class* object, MethodType m, Arg1 arg1, Arg2 arg2) :
        _object(object),
        _m(m),
        _arg1(arg1),
        _arg2(arg2) {
        }
    ~MethodClosure() {
    }

    virtual void Run() {
        std::cout << "address of object in Run : " << _object << std::endl;
        (_object->*_m)(_arg1, _arg2);
        delete this;
    }
private:
    Class* _object;
    MethodType _m;
    Arg1 _arg1;
    Arg2 _arg2;
};//MethodClosure1
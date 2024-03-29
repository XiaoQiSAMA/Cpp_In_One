#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include "mprpcapplication.h"
#include "mprpccontroller.h"
#include "zookeeperutil.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

/**
 * 本地stub代理对象在调用rpc方法时，需要给予相应方法的数据和参数来发送给代理方
 * 需发送数据: header_size + [service_name, method_name, args_size] + args
 * 
 * main:
 *  1. rpc请求的数据组装，数据序列化
 *  2. 发送rpc请求，wait
 *  3. 接收rpc响应
 *  4. 响应的反序列化
*/
// 所有通过stub代理对象调用的rpc方法，都走到这，统一做rpc方法调用的数据序列化和网络发送
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, 
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response, 
                    google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    // 获取参数的序列化字符串长度 args_size
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str)) {
        args_size = args_str.size();
    } else {
        controller->SetFailed("serialize request error!");
        // std::cout << "serialize request error!" << std::endl;
        return;
    }

    // 定义rpc的请求header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str)) {
        header_size = rpc_header_str.size();
    } else {
        controller->SetFailed("serialize rpc header error!");
        // std::cout << "serialize rpc header error!" << std::endl;
        return;
    }

    // 组织待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));

    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    // Debug
    std::cout << "====================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "====================================" << std::endl;

    // 使用tcp编程，完成rpc方法的远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd) {
        char errTxt[512] = {0};
        sprintf(errTxt, "create socket error! errno:%d", errno);
        controller->SetFailed(errTxt);
        return;
        // std::cout << "create socket error! errno: " << errno << std::endl;
        // exit(EXIT_FAILURE);
    }

    // // 读取配置文件的地址信息
    // std::string ip = MprpcApplication::GetConfig().Load("rpcserviceip");
    // uint16_t port = atoi(MprpcApplication::GetConfig().Load("rpcserviceport").c_str());

    // rpc调用方想调用service_name的method_name服务，需要查询zk上该服务所在的host信息
    ZkClient zkCli;
    zkCli.start();
    // 构建path
    std::string method_path = "/" + service_name + "/" + method_name;
    // 127.0.0.1:8000
    std::string host_data = zkCli.GetData(method_path.c_str());
    if (host_data == "") {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_data.find(":");
    if (idx == -1) {
        controller->SetFailed(method_path + " address is invalid!");
        return;
    }
    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx + 1, host_data.size() - idx).c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 连接rpc服务节点
    if (-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        char errTxt[512] = {0};
        sprintf(errTxt, "connect socket error! errno:%d", errno);
        controller->SetFailed(errTxt);
        // std::cout << "connect error! errno : " << errno << std::endl;
        close(clientfd);
        // exit(EXIT_FAILURE);
        return;
    }

    // 发送给rpc服务节点数据
    if (-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0)) {
        // std::cout << "send error! errno : " << errno << std::endl;
        char errTxt[512] = {0};
        sprintf(errTxt, "send socket error! errno:%d", errno);
        controller->SetFailed(errTxt);
        close(clientfd);
        return;
    }

    // 接收rpc请求的响应
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0))) {
        // std::cout << "recv error! errno : " << errno << std::endl;
        char errTxt[512] = {0};
        sprintf(errTxt, "recv socket error! errno:%d", errno);
        controller->SetFailed(errTxt);
        close(clientfd);
        return;
    }

    // std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromArray(recv_buf, recv_size)) {
        // std::cout << "parse error! response_str : " << recv_buf << std::endl;
        char errTxt[512] = {0};
        sprintf(errTxt, "parse error! response_str:%s", recv_buf);
        controller->SetFailed(errTxt);
        close(clientfd);
        return;
    }

    close(clientfd);
}
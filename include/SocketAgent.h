#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

class SocketAgent {
public:
    SocketAgent(int port); // 构造函数，传入端口号,创建socket
    SocketAgent(int port, bool nonBlock); // 构造函数，传入端口号,非阻塞
    virtual ~SocketAgent();
    virtual void initSocket();    // 初始化 Socket, 对于server端来说就是 bind 和 listen；对于client端来说就是 connect

    // void handleEvents();    // 处理 epoll 事件
    // // 其他 Socket 相关功能
protected:
    int socketFd;
    int epollFd; // 暂时没用
    int port;

    virtual int sendOp(char *buffer, int len);  // 发送数据
    virtual int recvOp();  // 接收数据
    virtual int closeOp(); // 关闭 Socket
};

// 在epoll中传入一个socket对象，然后在epoll中注册该socket对象
// 然后在epoll中监听该socket对象的事件，当该socket对象发生事件时
// epoll会通知该socket对象，然后该socket对象调用handleEvents()函数处理事件
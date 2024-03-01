/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 01:08:10
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 03:07:10
 */
// #include "InetAddress.h"
// class Socket
// {
// private:
//     const int fd_;
// public:
//     Socket(int fd);
//     ~Socket();

//     int fd() const;

//     // 设置listenFd的属性
//     void setReuseAddr(bool on); // 必须的，地址复用, 适用于服务器重启后立即启动, 但是会增加网络负载, 降低服务器性能
//     void setReusePort(bool on); // 端口复用, 适用于服务器重启后立即启动, 但是会增加网络负载, 降低服务器性能
//     void setKeepAlive(bool on); // 开启TCP心跳机制, 保持连接状态, 防止连接断开, 适用于长连接场景, 如聊天室, 直播等, 但是会增加网络负载, 降低服务器性能
//     void setTcpNoDelay(bool on); // 必须的，禁用Nagle算法, 有数据就发, 不用等待, 适用于实时性要求高的场景, 如游戏, 视频会议等

//     void bindAddress(const InetAddress &serverAddress); // 服务端的socket将调用该函数，绑定一个地址供外界连接
//     void listen(int num= SOMAXCONN); // 服务端的socket将调用该函数，监听一个地址； 可排队等待的连接数
//     int accept(InetAddress &peerAddress); // 服务端的socket将调用该函数，接受一个连接 
    
//     void connect(const InetAddress &serverAddress); // 客户端的socket将调用该函数，连接一个地址
//     void shutdownWrite(); // 关闭写s端

// };
#include "Socket.h"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/tcp.h>



int createNonblocking(){
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0){
        perror("socket() failed");
        printf("%s:%s:%d listen scoket create errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
    return sockfd;
}


Socket::Socket(int fd):fd_(fd){};

Socket::~Socket(){
    ::close(fd_);
};

void Socket::setReuseAddr(bool on){
    int optval = on ? 1 : 0;
    if(::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval)) < 0){
        perror("setsockopt() failed");
        printf("%s:%s:%d setsockopt SO_REUSEADDR errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
}

void Socket::setReusePort(bool on){
    int optval = on ? 1 : 0;
    if(::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval)) < 0){
        perror("setsockopt() failed");
        printf("%s:%s:%d setsockopt SO_REUSEPORT errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
}

void Socket::setKeepAlive(bool on){
    int optval = on ? 1 : 0;
    if(::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval)) < 0){
        perror("setsockopt() failed");
        printf("%s:%s:%d setsockopt SO_KEEPALIVE errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
}

void Socket::setTcpNoDelay(bool on){
    int optval = on ? 1 : 0;
    if(::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval)) < 0){
        perror("setsockopt() failed");
        printf("%s:%s:%d setsockopt TCP_NODELAY errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
}

void Socket::bindAddress(const InetAddress &serverAddress){
    if(::bind(fd_, serverAddress.addr(), sizeof(sockaddr)) < 0){
        perror("bind() failed");
        printf("%s:%s:%d bind errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
}

void Socket::listen(int num){
    if(::listen(fd_, num) < 0){
        perror("listen() failed");
        printf("%s:%s:%d listen errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(1);
    }
}

int Socket::accept(InetAddress &clientAddress){
    sockaddr_in peerAddress;
    socklen_t addrlen = sizeof(peerAddress);
    int connfd = ::accept4(fd_,(sockaddr *) &peerAddress, &addrlen,SOCK_NONBLOCK | SOCK_CLOEXEC);
    if(connfd == -1){
        perror("accept() failed");
        printf("%s:%s:%d accept errno: %d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        // exit(1);
        return -1; // 返回-1表示出错
    }
    // clientAddress = InetAddress(peerAddress); // 这样也可以吧？
    clientAddress.setAddr(peerAddress); // 设置客户端的地址
    return connfd;
}

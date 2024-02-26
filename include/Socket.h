#ifndef MY_SOCKET
#define MY_SOCKET

#include<unistd.h>
/**
 *  封装socket类： Socket类是socket 文件描述符（sock fd）的一个轻量级封装，提供操作底层sock fd的常用方法。
 * 采用RTII方式管理sock fd，但本身并不创建sock fd，也不打开它，只负责关闭。
 * 提供的public方法主要包括：获取tcp协议栈信息（tcp_info）；绑定ip地址（bind）；监听套接字（listen）；接收连接请求（accept）；关闭连接写方向（shutdown），等等。
 * 值得注意的是，Socket并不提供close sock fd的public方法，因为析构时，调用close关闭套接字
*/

class Socket
{
private:
    const int sockfd_;
public:
    explicit Socket(int sockfd) : sockfd_(sockfd)
    { }
    ~Socket();

    int fd() const { return sockfd_; }

      // abort if address in use
    void bindAddress(const InetAddress& addr);
    /* 监听socket fd, 核心调用listen(2).
     * 失败则终止程序
     */
    // abort if address in use
    void listen();

    /**
     * 接受连接请求, 返回conn fd(连接文件描述符). 核心调用accept(2).
     */
    /**
     * On success, returns a non-negative integer that is
     * a descriptor for the accepted socket, which has been
     * set to non-blocking and close-on-exec. *peeraddr is assigned.
     * On error, -1 is returned, and *peeraddr is untouched.
     */
    int accept(InetAddress* peeraddr);

};

Socket::~Socket()
{
    close(sockfd_);
}
#endif MY_SOCKET
/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-24 00:15:24
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-24 00:16:05
 */
/**
* socket fd 包装类
*　RAII机制管理socket fd: 对close socket fd负责, 但不包括open/create.
* 监听socket, 常用于server socket.
*/
class Socket 
{
public:
    explicit Socket(int sockfd)
    : sockfd_(sockfd)
    { }

    // Socket(Socket&&) // move ctor in c++11
    ~Socket();

    int fd() const { return sockfd_; }
    /* 获取tcp信息, 存放到tcp_info结构 */
    // return true if success.
    bool getTcpInfo(struct tcp_info*) const;
    /* 获取tcp信息字符串形式(NUL结尾), 存放到字符串数组buf[len] */
    bool getTcpInfoString(char* buf, int len) const;

    /* 绑定socket fd与本地ip地址,端口, 核心调用bind(2).
     * 失败则终止程序
     */
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

    /**
     * 关闭连接写方向
     */
    void shutdownWrite();

    /**
     * Enable/disable TCP_NODELAY
     */
    void setTcpNoDelay(bool on);

    /**
     * Enable/disable SO_REUSEADDR
     */
    void setReuseAddr(bool on);

    /**
     * Enable/disable SO_REUSEPORT
     */
    void setReusePort(bool on);

    /**
     * Enable/disable SO_KEEPALIVE
     * @param on
     */
    void setKeepAlive(bool on);

private:
    const int sockfd_;
};

/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 00:49:22
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 03:21:39
 */

#include "InetAddress.h"

int createNonblocking();

class Socket
{
private:
    const int fd_;
public:
    Socket(int fd);
    ~Socket();

    int fd() const{
        return fd_;
    };

    // 设置listenFd的属性
    void setReuseAddr(bool on); // 必须的，地址复用, 适用于服务器重启后立即启动, 但是会增加网络负载, 降低服务器性能
    void setReusePort(bool on); // 端口复用, 适用于服务器重启后立即启动, 但是会增加网络负载, 降低服务器性能
    void setKeepAlive(bool on); // 开启TCP心跳机制, 保持连接状态, 防止连接断开, 适用于长连接场景, 如聊天室, 直播等, 但是会增加网络负载, 降低服务器性能
    void setTcpNoDelay(bool on); // 必须的，禁用Nagle算法, 有数据就发, 不用等待, 适用于实时性要求高的场景, 如游戏, 视频会议等

    void bindAddress(const InetAddress &serverAddress); // 服务端的socket将调用该函数，绑定一个地址供外界连接
    void listen(int num= SOMAXCONN); // 服务端的socket将调用该函数，监听一个地址； 可排队等待的连接数
    int accept(InetAddress &peerAddress); // 服务端的socket将调用该函数，接受一个连接 
    
    // void connect(const InetAddress &serverAddress); // 客户端的socket将调用该函数，连接一个地址
    // void shutdownWrite(); // 关闭写s端

};

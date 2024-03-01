/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-01 16:42:23
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 02:54:27
 */



#include <netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>

#include <string>

class InetAddress
{
private:
    sockaddr_in addr_;
public:
    InetAddress();
    InetAddress(const std::string &ip, int port); // 通过字符串表示的地址和整数表示的端口号构造
    InetAddress(const sockaddr_in &addr);// 服务器端对连接上的客户端分配的地址
    InetAddress(in_addr_t ip,  int port);
    
    const char *ip() const; // 返回字符串表示的地址
    uint16_t port() const; // 返回整数表示的端口号
    const sockaddr *addr() const; // 返回sockaddr_in结构体
    void setAddr(const sockaddr_in &addr);
    ~InetAddress();
};

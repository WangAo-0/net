/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-01 22:03:57
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 02:55:51
 */
#include "InetAddress.h"

InetAddress::InetAddress()
{

}

InetAddress::InetAddress(const std::string &ip, int port) // 如果是监听的fd，用这个函数
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);    
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr); // addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(in_addr_t ip, int port) // 如果是监听的fd，用这个函数
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);    
    addr_.sin_addr.s_addr = ip; 
}

InetAddress::InetAddress(const sockaddr_in &addr ):addr_(addr){};// 如果是连接的fd，用这个函数

InetAddress::~InetAddress(){};

const char *InetAddress::ip() const 
{
    return inet_ntoa(addr_.sin_addr); // 将二进制的ip地址转为字符串
}

uint16_t InetAddress::port() const
{
    return ntohs(addr_.sin_port); // 将端口号由网络字节序转为主机字节序
}

const sockaddr *InetAddress::addr() const
{
    return (sockaddr *)&addr_; // 将sockaddr_in结构体转为sockaddr结构体，返回addr_的地址
}

void InetAddress::setAddr(const sockaddr_in &addr)
{
    addr_ = addr;
}
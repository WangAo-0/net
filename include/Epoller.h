/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-28 23:05:24
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-01 09:37:12
 */
#include <sys/epoll.h>
#include <sys/socket.h>
class Epoller
{
private:
    int epollFd_;
public:
    Epoller(/* args */);
    ~Epoller();
};

Epoller::Epoller(/* args */)
{
}

Epoller::~Epoller()
{
}

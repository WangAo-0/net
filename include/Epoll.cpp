/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 03:43:26
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 08:58:42
 */

// #include <sys/epoll.h>
// #include <vector>

// class Epoll
// {
// private:
//     static const int MAXFDS = 100000;
//     int epollFd_ = -1;
//     epoll_event *events_[MAXFDS];
// public:
//     Epoll(); // 创建一个epoll实例
//     ~Epoll(); // 关闭epoll实例

//     void addFd(int fd, uint32_t events); // 添加一个文件描述符到epoll实例(红黑树)
//     void modFd(int fd, uint32_t events); // 修改一个文件描述符在epoll实例中的事件
//     std::vector<epoll_event> loop (int timeout = -1);
// };
#include "Epoll.h"
#include <cerrno>
#include <string.h>
#include<unistd.h>
#include<cstdlib>
#include<cstdio>

Epoll::Epoll()
{
    epollFd_ = epoll_create1(0);
    if (epollFd_ == -1)
    {
        printf("epoll_create1() failed (%d)",errno);
        exit(EXIT_FAILURE);
    }
}

Epoll::~Epoll()
{
    close(epollFd_);
}

int Epoll::addFd(int socketFd, uint32_t events){
    epoll_event event;
    event.events = events;
    event.data.fd = socketFd;

    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, socketFd, &event) == -1)
    {
        printf("epoll_ctl() failed (%d)",errno);
       // exit(EXIT_FAILURE);
       return -1;
    }
    return 0;
}

int Epoll::delFd(int socketFd){
    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, socketFd, NULL) == -1)
    {
        printf("epoll_ctl() failed (%d)",errno);
       // exit(EXIT_FAILURE);
       return -1;
    }
    return 0;
}

int Epoll::modFd(int socketFd, uint32_t events){
    epoll_event event;
    event.events = events;
    event.data.fd = socketFd;

    if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, socketFd, &event) == -1)
    {
        printf("epoll_ctl() failed (%d)",errno);
       // exit(EXIT_FAILURE);
       return -1;
    }
    return 0;
}

std::vector<epoll_event> Epoll::loop (int timeout){
    std::vector<epoll_event> events; // 用于存放epoll_wait返回的事件
    memset(events_, 0, sizeof(events_));
    int nfds = epoll_wait(epollFd_, events_, MAXFDS, timeout);
    if (nfds == -1)
    {
        printf("epoll_wait() failed (%d)",errno);
        exit(EXIT_FAILURE);
    }

    if (nfds == 0){
        printf("epoll_wait() timeout");
        return events;
    }

    for (int i = 0; i < nfds; i++)
    {
        events.push_back(events_[i]);
    }
    return events;
}

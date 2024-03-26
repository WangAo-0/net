/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 03:43:26
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-26 13:46:57
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
#include "Channel.h"

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

// int Epoll::addFd(int socketFd, uint32_t events){
//     epoll_event event;
//     event.events = events;
//     event.data.fd = socketFd;

//     if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, socketFd, &event) == -1)
//     {
//         printf("epoll_ctl() failed (%d)",errno);
//        // exit(EXIT_FAILURE);
//        return -1;
//     }
//     return 0;
// }

int Epoll::updateChannel(Channel *channel){
    epoll_event event; // 声明事件的结构体
    event.events = channel->events(); // 设置事件
    event.data.ptr = channel; // 设置channel

    if (channel->inEpoll()){ // 如果channel已经在epoll树上，则调用EPOLL_CTL_MOD修改
        if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, channel->fd(), &event) == -1)
        {
            printf("epoll_ctl() EPOLL_CTL_MOD failed (%d)",errno);
           // exit(EXIT_FAILURE);
           return -1;
        }
    }else{ // 如果channel不在epoll树上，则添加
        if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, channel->fd(), &event) == -1)
        {
            printf("epoll_ctl() EPOLL_CTL_ADD failed (%d)",errno);
           // exit(EXIT_FAILURE);
           return -1;
        }
        channel->setInEpoll(); // 设置channel在epoll树上
    }
    return 0;

}


int Epoll::delFd(int socketFd){
    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, socketFd, NULL) == -1)
    {
        printf("epoll_ctl() EPOLL_CTL_DEL failed (%d)",errno);
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
        printf("epoll_ctl() EPOLL_CTL_MOD failed (%d)",errno);
       // exit(EXIT_FAILURE);
       return -1;
    }
    return 0;
}


// std::vector<epoll_event> Epoll::loop (int timeout){
//     std::vector<epoll_event> events; // 用于存放epoll_wait返回的事件
//     memset(events_, 0, sizeof(events_));
//     int nfds = epoll_wait(epollFd_, events_, MAXFDS, timeout);
//     if (nfds == -1)
//     {
//         printf("epoll_wait() failed (%d)",errno);
//         exit(EXIT_FAILURE);
//     }

//     if (nfds == 0){
//         printf("epoll_wait() timeout");
//         return events;
//     }

//     for (int i = 0; i < nfds; i++)
//     {
//         events.push_back(events_[i]);
//     }
//     return events;
// }

std::vector<Channel*> Epoll::loop (int timeout){
    std::vector<Channel*> channels; // 用于存放epoll_wait返回的事件
    memset(events_, 0, sizeof(events_));
    int nfds = epoll_wait(epollFd_, events_, MAXFDS, timeout);
    if (nfds == -1)
    {
        printf("epoll_wait() failed (%d)",errno);
        exit(EXIT_FAILURE);
    }

    if (nfds == 0){
        printf("epoll_wait() timeout");
        return channels;
    }

    for (int i = 0; i < nfds; i++)
    {
        Channel * ch = static_cast<Channel*>(events_[i].data.ptr);
        channels.push_back(ch);
        ch->setREvents(events_[i].events);
    }
    return channels;
}